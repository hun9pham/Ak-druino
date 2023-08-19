//-----------------------------------------------------------------------------
// Project   :  Command Line Parser
// Author    :  HungPNQ
// Date      :  30/4/2022
// Brief     :
//				- Base on ThanNT cmd_line.h
//-----------------------------------------------------------------------------

#ifndef __CMD_LINE_H
#define __CMD_LINE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define MAX_CMD_SIZE		(32)

enum eCMD_CodeReturn {
	CMD_SUCCESS = 0,
	CMD_PARSER_FAILED,
	CMD_TBL_NOT_FOUND,
	CMD_NOT_FOUND,
	CMD_TOO_LONG,
};

typedef int8_t (*pfCmdFunc)(uint8_t* argv);

typedef struct {
	const int8_t* cmd;
	pfCmdFunc func;
	const int8_t* Str;
} cmdLineStruct_t;

class CmdLine {
private:
	cmdLineStruct_t *m_tblCLi = NULL;
	uint8_t m_buf[MAX_CMD_SIZE];
	uint8_t m_id;
	uint8_t m_parserCounter;
public:
	CmdLine();
	~CmdLine();

	void tblCmdLineInit(const cmdLineStruct_t *tblCLi);
	void putChar(uint8_t let);
	uint8_t getChar();
	uint8_t parserChar(char delimiter);
	uint8_t getParserCount();
	uint8_t* getKeyId(uint8_t id);
	void Refresh();
};

extern CmdLine CMDLINE;

#ifdef __cplusplus
}
#endif

#endif /* __CMD_LINE_H */