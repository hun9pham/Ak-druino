//-----------------------------------------------------------------------------
// Project   :  Command Line Parser
// Author    :  HungPNQ
// Date      :  30/4/2022
// Brief     :
//				- Reference from ThanNT cmd_line.h
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "cmd_line.h"

CmdLine CMDLINE;

CmdLine::CmdLine() {

}

CmdLine::~CmdLine() {
	m_tblCLi = NULL;
}

void CmdLine::tblCmdLineInit(const cmdLineStruct_t *tblCLi) {
	if (tblCLi != NULL) {
		m_tblCLi = (cmdLineStruct_t*)tblCLi;
	}
	memset(m_buf, 0, MAX_CMD_SIZE);
	m_id = m_parserCounter = 0;
}

void CmdLine::putChar(uint8_t let) {
	if (m_id < MAX_CMD_SIZE) {
		m_buf[m_id++] = let;
	}
}

uint8_t CmdLine::getChar() {
	uint8_t ret = 0;
	if (m_id > 0) {
		ret = m_buf[m_id];
		--m_id;
	}
	return ret;
}

uint8_t CmdLine::parserChar(char delimiter) {
	uint8_t ret = CMD_SUCCESS;
	uint8_t *keyCompare = NULL;
	uint8_t tblCLiIndex = 0;

	if (m_tblCLi == NULL) {
		ret = CMD_TBL_NOT_FOUND;
		goto END_OF_PARSER_PROCESS;
	}

	if (m_id == 0) {
		ret = CMD_PARSER_FAILED;
		goto END_OF_PARSER_PROCESS;
	}

	for (uint8_t id = 0; id < m_id; ++id) {
		if (m_buf[id] == delimiter || m_buf[id] == '\r' || m_buf[id] == '\n') {
			m_buf[id] = 0;
			++m_parserCounter;
		}
	}

	keyCompare = &m_buf[0];
	while (m_tblCLi[tblCLiIndex].cmd) {
		if (strcmp((const char*)m_tblCLi[tblCLiIndex].cmd, (const char*)keyCompare) == 0) {
			m_tblCLi[tblCLiIndex].func(keyCompare);
			break;
		}
		++tblCLiIndex;
	}

END_OF_PARSER_PROCESS:
	CmdLine::Refresh();
	return ret;
}

uint8_t CmdLine::getParserCount() {
	return m_parserCounter;
}

uint8_t* CmdLine::getKeyId(uint8_t id) {
	uint8_t *ret = m_buf;
	if (id != 0) {
		while (id) {
			ret += strlen((char *)ret) + 1;
			--id;
		}
	}
	return ret;
}

void CmdLine::Refresh() {
	memset(m_buf, 0, MAX_CMD_SIZE);
	m_id = m_parserCounter = 0;
}
