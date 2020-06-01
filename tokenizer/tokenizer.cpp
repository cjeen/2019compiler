#include "tokenizer/tokenizer.h"

#include <cctype>
#include <sstream>

namespace c0 {

	std::pair<std::optional<Token>, std::optional<CompilationError>> Tokenizer::NextToken() {
		if (!_initialized)
			readAll();
		if (_rdr.bad())
			return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(0, 0, ErrorCode::ErrStreamError));
		if (isEOF())
			return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(0, 0, ErrorCode::ErrEOF));
		auto p = nextToken();
		if (p.second.has_value())
			return std::make_pair(p.first, p.second);
		auto err = checkToken(p.first.value());
		if (err.has_value())
			return std::make_pair(p.first, err.value());
		return std::make_pair(p.first, std::optional<CompilationError>());
	}

	std::pair<std::vector<Token>, std::optional<CompilationError>> Tokenizer::AllTokens() {
		std::vector<Token> result;
		while (true) {
			auto p = NextToken();
			if (p.second.has_value()) {
				if (p.second.value().GetCode() == ErrorCode::ErrEOF)
					return std::make_pair(result, std::optional<CompilationError>());
				else
					return std::make_pair(std::vector<Token>(), p.second);		
			}
			result.emplace_back(p.first.value());
		}
	}

	// 注意：这里的返回值中 Token 和 CompilationError 只能返回一个，不能同时返回。
	std::pair<std::optional<Token>, std::optional<CompilationError>> Tokenizer::nextToken() {
		// 用于存储已经读到的组成当前token字符
		std::stringstream ss;
		// 分析token的结果，作为此函数的返回值
		std::pair<std::optional<Token>, std::optional<CompilationError>> result;
		// <行号，列号>，表示当前token的第一个字符在源代码中的位置
		std::pair<int64_t, int64_t> pos;
		// 记录当前自动机的状态，进入此函数时是初始状态
		DFAState current_state = DFAState::INITIAL_STATE;
		// 这是一个死循环，除非主动跳出
		// 每一次执行while内的代码，都可能导致状态的变更
		while (true) {
			// 读一个字符，请注意auto推导得出的类型是std::optional<char>
			// 这里其实有两种写法
			// 1. 每次循环前立即读入一个 char
			// 2. 只有在可能会转移的状态读入一个 char
			// 因为我们实现了 unread，为了省事我们选择第一种
			auto current_char = nextChar();
			// 针对当前的状态进行不同的操作
			switch (current_state) {

				// 初始状态
				// 这个 case 我们给出了核心逻辑，但是后面的 case 不用照搬。
			case INITIAL_STATE: {
				//clear cache
				ss.str("");
				// 已经读到了文件尾
				if (!current_char.has_value())
					// 返回一个空的token，和编译错误ErrEOF：遇到了文件尾
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(0, 0, ErrEOF));

				// 获取读到的字符的值，注意auto推导出的类型是char
				auto ch = current_char.value();
				// 标记是否读到了不合法的字符，初始化为否
				auto invalid = false;

				// 使用了自己封装的判断字符类型的函数，定义于 tokenizer/utils.hpp
				// see https://en.cppreference.com/w/cpp/string/byte/isblank
				if (c0::isspace(ch)) // 读到的字符是空白字符（空格、换行、制表符等）
					current_state = DFAState::INITIAL_STATE; // 保留当前状态为初始状态，此处直接break也是可以的
				else if (!c0::isprint(ch)) // control codes and backspace
					invalid = true;	
				else if (c0::isdigit(ch)) // 读到的字符是数字
				{
					if(ch == '0') {
						current_state = DFAState::ZERO_STATE;
					}
					else {
						current_state = DFAState::DEC_STATE;
					}
				}
					
				else if (c0::isalpha(ch)) // 读到的字符是英文字母
					current_state = DFAState::IDENTIFIER_STATE; // 切换到标识符的状态
				else {
					switch (ch) {
					case '_': 
						current_state = DFAState::XHX_STATE;
						break;
					case '(': 
						current_state = DFAState::YKHZ_STATE;
						break;
					case ')': 
						current_state = DFAState::YKHY_STATE;
						break;
					case '[': 
						current_state = DFAState::FKHZ_STATE;
						break;
					case ']': 
						current_state = DFAState::FKHY_STATE;
						break;
					case '{': 
						current_state = DFAState::HKHZ_STATE;
						break;
					case '}': 
						current_state = DFAState::HKHY_STATE;
						break;
					case '<': 
						current_state = DFAState::XYH_STATE;
						break;
					case '=': 
						current_state = DFAState::DH_STATE;
						break;
					case '>': 
						current_state = DFAState::DYH_STATE;
						break;
					case '.': 
						current_state = DFAState::DIAN1_STATE;
						break;
					case ',': 
						current_state = DFAState::DOU_STATE;
						break;
					case ':': 
						current_state = DFAState::MH_STATE;
						break;
					case ';': 
						current_state = DFAState::FH_STATE;
						break;
					case '!': 
						current_state = DFAState::TH_STATE;
						break;
					case '?': 
						current_state = DFAState::WH_STATE;
						break;
					case '+': 
						current_state = DFAState::JIA_STATE;
						break;
					case '-': 
						current_state = DFAState::JIAN_STATE;
						break;
					case '*': 
						current_state = DFAState::CHENG_STATE;
						break;
					case '/': 
						current_state = DFAState::CHU_STATE;
						break;
					case '%':
						current_state = DFAState::MO_STATE;
						break;
					case '^':
						current_state = DFAState::SJ_STATE;
						break;
					case '&':
						current_state = DFAState::YU_STATE;
						break;
					case '|':
						current_state = DFAState::HUO_STATE;
						break;
					case '~':
						current_state = DFAState::FAN_STATE;
						break;
					case '\\':
						current_state = DFAState::FXG_STATE;
						break;
					case '\"':
						current_state = DFAState::SYH_STATE;
						break;
					case '\'':
						current_state = DFAState::YH_STATE;
						break;
					case '`':
						current_state = DFAState::FDYH_STATE;
						break;
					case '$':
						current_state = DFAState::DOLLAR_STATE;
						break;
					case '#':
						current_state = DFAState::JING_STATE;
						break;
					case '@':
						current_state = DFAState::AT_STATE;
						break;

					///// 请填空：
					///// 对于其他的可接受字符
					///// 切换到对应的状态

					// 不接受的字符导致的不合法的状态
					default:
						invalid = true;
						break;
					}
				}
				// 如果读到的字符导致了状态的转移，说明它是一个token的第一个字符
				if (current_state != DFAState::INITIAL_STATE)
					pos = previousPos(); // 记录该字符的的位置为token的开始位置
				// 读到了不合法的字符
				if (invalid) {
					// 回退这个字符
					unreadLast();
					// 返回编译错误：非法的输入
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidInput));
				}
				// 如果读到的字符导致了状态的转移，说明它是一个token的第一个字符
				if (current_state != DFAState::INITIAL_STATE) // ignore white spaces
					ss << ch; // 存储读到的字符
				break;
			}

								// 当前状态是无符号整数
			case ZERO_STATE: {
				// 请填空：
				// 如果当前已经读到了文件尾，则解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				// 如果读到的字符是数字，则存储读到的字符
				// 如果读到的是字母，则存储读到的字符，并切换状态到标识符
				// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				if (!current_char.has_value())
				{
					int32_t tmp;
					std::string jud;
					ss >> jud;
					int i;
					for (i = 0; !(jud[i] - '0') && i != jud.length() - 1; ++i);
					for (int j = 0; j < jud.substr(i).length(); ++j) {
						tmp = tmp * 10 + (jud.substr(i)[j] - '0');
					}
					return std::make_pair(std::make_optional<Token>(TokenType::UNSIGNED_INTEGER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
					auto ch = current_char.value();

				if (c0::isdigit(ch))
				{// 读到的字符是数字
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidInteger));
				}
				else if (c0::isalpha(ch))
				{// 读到的字符是英文字母
					if (ch == 'x' || ch == 'X') {
						ss << ch;
						current_state = DFAState::PREHEX_STATE;
					}
					else if (ch == 'e' || ch == 'E') {
						ss << ch;
						current_state = DFAState::EXE_STATE;
					}
					else {
						ss << ch;
						current_state = DFAState::IDENTIFIER_STATE; // 切换到标识符的状态
					}
				}
				else if (ch == '.') {
					ss << ch;
					current_state = DFAState::DIAN2_STATE;
				}
				else {
					unreadLast();
					int32_t tmp=0;
					std::string jud;
					ss >> jud;
					int i;
					for (i = 0; !(jud[i] - '0')&&i!=jud.length()-1; ++i);
					for (int j = 0; j < jud.substr(i).length(); ++j) {
						tmp = tmp * 10 + (jud.substr(i)[j] - '0');
					}
					return std::make_pair(std::make_optional<Token>(TokenType::UNSIGNED_INTEGER,tmp,pos, currentPos()), std::optional<CompilationError>());
				}
				break;
			}
			case DEC_STATE: {
				// 请填空：
				// 如果当前已经读到了文件尾，则解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				// 如果读到的字符是数字，则存储读到的字符
				// 如果读到的是字母，则存储读到的字符，并切换状态到标识符
				// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				if (!current_char.has_value())
				{
					int32_t tmp;
					std::string jud;
					ss >> jud;
					int i;
					for (i = 0; !(jud[i] - '0') && i != jud.length() - 1; ++i);
					for (int j = 0; j < jud.substr(i).length(); ++j) {
						tmp = tmp * 10 + (jud.substr(i)[j] - '0');
					}
					return std::make_pair(std::make_optional<Token>(TokenType::UNSIGNED_INTEGER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				auto ch = current_char.value();

				if (c0::isdigit(ch))
				{// 读到的字符是数字
					ss << ch;
				}
				else if (c0::isalpha(ch))
				{// 读到的字符是英文字母
					if (ch == 'e' || ch == 'E') {
						ss << ch;
						current_state = DFAState::EXE_STATE;
					}
					else {
						ss << ch;
						current_state = DFAState::IDENTIFIER_STATE; // 切换到标识符的状态
					}
				}
				else if (ch == '.'){
					ss << ch;
						current_state = DFAState::DIAN2_STATE;
				}
				else {
					unreadLast();
					int32_t tmp = 0;
					std::string jud;
					ss >> jud;
					int i;
					for (i = 0; !(jud[i] - '0') && i != jud.length() - 1; ++i);
					for (int j = 0; j < jud.substr(i).length(); ++j) {
						tmp = tmp * 10 + (jud.substr(i)[j] - '0');
					}
					return std::make_pair(std::make_optional<Token>(TokenType::UNSIGNED_INTEGER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				break;
			}
			case PREHEX_STATE: {
				// 请填空：
				// 如果当前已经读到了文件尾，则解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				// 如果读到的字符是数字，则存储读到的字符
				// 如果读到的是字母，则存储读到的字符，并切换状态到标识符
				// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				if (!current_char.has_value())
				{
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidInteger));
				}
				auto ch = current_char.value();

				if (c0::isdigit(ch))
				{// 读到的字符是数字
					ss << ch;
					current_state = DFAState::HEX_STATE;

				}
				else if (c0::isalpha(ch))
				{// 读到的字符是英文字母
					ss << ch;
					switch (ch) {
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
						current_state = DFAState::HEX_STATE;
						break;
					default:
						current_state = DFAState::IDENTIFIER_STATE; // 切换到标识符的状态
						break;
					}
				}
				else {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidInteger));
				}
				break;
			}
			case HEX_STATE: {
				// 请填空：
				// 如果当前已经读到了文件尾，则解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				// 如果读到的字符是数字，则存储读到的字符
				// 如果读到的是字母，则存储读到的字符，并切换状态到标识符
				// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串为整数
				//     解析成功则返回无符号整数类型的token，否则返回编译错误
				if (!current_char.has_value())
				{
					int32_t tmp;
					std::string jud;
					ss >> jud;
					int i;
					for (i = 2; !(jud[i] - '0') && i != jud.length() - 1; ++i);
					for (int j = 0; j < jud.substr(i).length(); ++j) {
						if (jud.substr(i)[j] >= 'a' && jud.substr(i)[j] <= 'f') {
							tmp = tmp * 16 + (jud.substr(i)[j] - 'a'+10);
						}
						else if (jud.substr(i)[j] >= '0' && jud.substr(i)[j] <= '9') {
							tmp = tmp * 16 + (jud.substr(i)[j] - '0');
						}
						else if (jud.substr(i)[j] >= 'A' && jud.substr(i)[j] <= 'F') {
							tmp = tmp * 16 + (jud.substr(i)[j] - 'A'+10);
						}
						
					}
					return std::make_pair(std::make_optional<Token>(TokenType::UNSIGNED_INTEGER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				auto ch = current_char.value();

				if (c0::isdigit(ch))
				{// 读到的字符是数字
					ss << ch;
				}
				else if (c0::isalpha(ch))
				{// 读到的字符是英文字母

					ss << ch;
					switch (ch) {
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
						break;
					default:
						current_state = DFAState::IDENTIFIER_STATE; // 切换到标识符的状态
						break;
					}

				}
				else {
					unreadLast();
					int32_t tmp = 0;
					std::string jud;
					ss >> jud;
					int i;
					for (i = 2; !(jud[i] - '0') && i != jud.length() - 1; ++i);
					for (int j = 0; j < jud.substr(i).length(); ++j) {
						if (jud.substr(i)[j] >= 'a' && jud.substr(i)[j] <= 'f') {
							tmp = tmp * 16 + (jud.substr(i)[j] - 'a' + 10);
						}
						else if (jud.substr(i)[j] >= '0' && jud.substr(i)[j] <= '9') {
							tmp = tmp * 16 + (jud.substr(i)[j] - '0');
						}
						else if (jud.substr(i)[j] >= 'A' && jud.substr(i)[j] <= 'F') {
							tmp = tmp * 16 + (jud.substr(i)[j] - 'A' + 10);
						}

					}
					return std::make_pair(std::make_optional<Token>(TokenType::UNSIGNED_INTEGER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				break;
			}
			case IDENTIFIER_STATE: {
				// 请填空：
				// 如果当前已经读到了文件尾，则解析已经读到的字符串
				//     如果解析结果是关键字，那么返回对应关键字的token，否则返回标识符的token
				// 如果读到的是字符或字母，则存储读到的字符
				// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串
				//     如果解析结果是关键字，那么返回对应关键字的token，否则返回标识符的token
				if (!current_char.has_value()) {
					std::string tmp;
					ss >> tmp;
					if (tmp == "const") {
						return std::make_pair(std::make_optional<Token>(TokenType::CONST, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "void") {
						return std::make_pair(std::make_optional<Token>(TokenType::VOID, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "int") {
						return std::make_pair(std::make_optional<Token>(TokenType::INT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "char") {
						return std::make_pair(std::make_optional<Token>(TokenType::CHAR, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "double") {
						return std::make_pair(std::make_optional<Token>(TokenType::DOUBLE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "struct") {
						return std::make_pair(std::make_optional<Token>(TokenType::STRUCT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "if") {
						return std::make_pair(std::make_optional<Token>(TokenType::IF, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "else") {
						return std::make_pair(std::make_optional<Token>(TokenType::ELSE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "switch") {
						return std::make_pair(std::make_optional<Token>(TokenType::SWITCH, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "case") {
						return std::make_pair(std::make_optional<Token>(TokenType::CASE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "default") {
						return std::make_pair(std::make_optional<Token>(TokenType::DEFAULT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "while") {
						return std::make_pair(std::make_optional<Token>(TokenType::WHILE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "for") {
						return std::make_pair(std::make_optional<Token>(TokenType::FOR, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "do") {
						return std::make_pair(std::make_optional<Token>(TokenType::DO, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "return") {
						return std::make_pair(std::make_optional<Token>(TokenType::RETURN, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "break") {
						return std::make_pair(std::make_optional<Token>(TokenType::BREAK, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "continue") {
						return std::make_pair(std::make_optional<Token>(TokenType::CONTINUE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "print") {
						return std::make_pair(std::make_optional<Token>(TokenType::PRINT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "scan") {
						return std::make_pair(std::make_optional<Token>(TokenType::SCAN, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else {
						if (!c0::isalpha(tmp[0])) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidIdentifier));
						}
						else {
							return std::make_pair(std::make_optional<Token>(TokenType::IDENTIFIER, tmp, pos, currentPos()), std::optional<CompilationError>());
						}
					}
				}
					
				auto ch = current_char.value();

				if (c0::isdigit(ch))
				{// 读到的字符是数字
					ss << ch;
				}
				else if (c0::isalpha(ch))
				{// 读到的字符是英文字母
					ss << ch;
				}
				else {
					unreadLast();
					std::string tmp;
					ss >> tmp;
					if (tmp == "const") {
						return std::make_pair(std::make_optional<Token>(TokenType::CONST, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "void") {
						return std::make_pair(std::make_optional<Token>(TokenType::VOID, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "int") {
						return std::make_pair(std::make_optional<Token>(TokenType::INT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "char") {
						return std::make_pair(std::make_optional<Token>(TokenType::CHAR, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "double") {
						return std::make_pair(std::make_optional<Token>(TokenType::DOUBLE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "struct") {
						return std::make_pair(std::make_optional<Token>(TokenType::STRUCT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "if") {
						return std::make_pair(std::make_optional<Token>(TokenType::IF, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "else") {
						return std::make_pair(std::make_optional<Token>(TokenType::ELSE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "switch") {
						return std::make_pair(std::make_optional<Token>(TokenType::SWITCH, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "case") {
						return std::make_pair(std::make_optional<Token>(TokenType::CASE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "default") {
						return std::make_pair(std::make_optional<Token>(TokenType::DEFAULT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "while") {
						return std::make_pair(std::make_optional<Token>(TokenType::WHILE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "for") {
						return std::make_pair(std::make_optional<Token>(TokenType::FOR, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "do") {
						return std::make_pair(std::make_optional<Token>(TokenType::DO, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "return") {
						return std::make_pair(std::make_optional<Token>(TokenType::RETURN, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "break") {
						return std::make_pair(std::make_optional<Token>(TokenType::BREAK, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "continue") {
						return std::make_pair(std::make_optional<Token>(TokenType::CONTINUE, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "print") {
						return std::make_pair(std::make_optional<Token>(TokenType::PRINT, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else if (tmp == "scan") {
						return std::make_pair(std::make_optional<Token>(TokenType::SCAN, tmp, pos, currentPos()), std::optional<CompilationError>());
					}
					else {
						if (!c0::isalpha(tmp[0])) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidIdentifier));
						}
						else {
							return std::make_pair(std::make_optional<Token>(TokenType::IDENTIFIER, tmp, pos, currentPos()), std::optional<CompilationError>());
						}
					}
				}
				break;
			}
			case EXE_STATE: {
				if (!current_char.has_value()) {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidDouble));
				}
				if (c0::isdigit(current_char.value())) {
					ss << current_char.value();
					current_state = DFAState::EXD_STATE;
				}
				else if (current_char.value() == '+' || current_char.value() == '-') {
					ss << current_char.value();
					current_state = DFAState::EXS_STATE;
				}
				else {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidDouble));
				}
				break;
			}
			case EXS_STATE: {
				if (!current_char.has_value()) {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidDouble));
				}
				if (c0::isdigit(current_char.value())) {
					ss << current_char.value();
					current_state = DFAState::EXD_STATE;
				}
				else {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidDouble));
				}
				break;
			}
			case EXD_STATE: {
				if (!current_char.has_value()) {
					double tmp;
					ss >> tmp;
					return std::make_pair(std::make_optional<Token>(TokenType::DOUBLELITER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				if (c0::isdigit(current_char.value())) {
					ss << current_char.value();
				}
				else {
					unreadLast();
					double tmp;
					ss >> tmp;
					return std::make_pair(std::make_optional<Token>(TokenType::DOUBLELITER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				break;
			}

								   // 如果当前状态是加号




			case XHX_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::XHX, '_', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case YKHZ_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::YKHZ, '(', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case YKHY_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::YKHY, ')', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case FKHZ_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::FKHZ, '[', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case FKHY_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::FKHY, ']', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case HKHZ_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::HKHZ, '{', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case HKHY_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::HKHY, '}', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case XYH_STATE: {
				if(current_char.has_value()&&current_char.value()=='=') {
					return std::make_pair(std::make_optional<Token>(TokenType::XYDY, std::string("<="), pos, currentPos()), std::optional<CompilationError>());
				}
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::XYH, '<', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case DH_STATE: {
				if (current_char.has_value() && current_char.value() == '=') {
					return std::make_pair(std::make_optional<Token>(TokenType::DDY, std::string("=="), pos, currentPos()), std::optional<CompilationError>());
				}
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::DH, '=', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case DYH_STATE: {
				if (current_char.has_value() && current_char.value() == '=') {
					return std::make_pair(std::make_optional<Token>(TokenType::DYDY, std::string(">="), pos, currentPos()), std::optional<CompilationError>());
				}
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::DYH, '>', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case DIAN1_STATE: {
				if (!current_char.has_value()) {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidDouble));
				}
				if (c0::isdigit(current_char.value())) {
					ss << current_char.value();
					current_state = DFAState::DIAND_STATE;
				}
				else {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidDouble));
				}
				break;
			}
			case DIAN2_STATE: {
				if (!current_char.has_value()) {
					double tmp;
					ss >> tmp;
					return std::make_pair(std::make_optional<Token>(TokenType::DOUBLELITER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				if (current_char.value() == 'e' || current_char.value() == 'E') {
					ss << current_char.value();
					current_state = DFAState::EXE_STATE;
				}
				else if (c0::isdigit(current_char.value())) {
					ss << current_char.value();
					current_state = DFAState::DIAND_STATE;
				}
				else {
					unreadLast();
					double tmp;
					ss >> tmp;
					return std::make_pair(std::make_optional<Token>(TokenType::DOUBLELITER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				break;
			}
			case DIAND_STATE: {
				if (!current_char.has_value()) {
					double tmp;
					ss >> tmp;
					return std::make_pair(std::make_optional<Token>(TokenType::DOUBLELITER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				if (current_char.value() == 'e' || current_char.value() == 'E') {
					ss << current_char.value();
					current_state = DFAState::EXE_STATE;
				}
				else if (c0::isdigit(current_char.value())) {
					ss << current_char.value();
				}
				else {
					unreadLast();
					double tmp;
					ss >> tmp;
					return std::make_pair(std::make_optional<Token>(TokenType::DOUBLELITER, tmp, pos, currentPos()), std::optional<CompilationError>());
				}
				break;
			}
			case DOU_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::DOU, ',', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case MH_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::MH, ':', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case FH_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::FH, ';', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case TH_STATE: {
				if (current_char.has_value() && current_char.value() == '=') {
					return std::make_pair(std::make_optional<Token>(TokenType::BDY, std::string("!="), pos, currentPos()), std::optional<CompilationError>());
				}
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::TH, '!', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case WH_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::WH, '?', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case JIA_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::JIA, '+', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case JIAN_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::JIAN, '-', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case CHENG_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::CHENG, '*', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case CHU_STATE: {
				if (current_char.has_value()) {
					if (current_char == '/') {
						while (1) {
							current_char = nextChar();
							if (!current_char.has_value()) {
								return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidComment));
							}
							if (current_char.value() == 0x0A|| current_char.value() == 0x0D) {
								current_state = DFAState::INITIAL_STATE;
								break;
							}
						}
						break;
					}
					else if (current_char == '*') {
						while (1) {
							current_char = nextChar();
							if (!current_char.has_value()) {
								return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidComment));
							}
							if (current_char.value() == '*') {
								current_char = nextChar();
								if (!current_char.has_value()) {
									return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidComment));
								}
								if (current_char.value() == '/') {
									current_state = DFAState::INITIAL_STATE;
									break;
								}
								unreadLast();
							}
						}
						break;
					}
				}
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::CHU, '/', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case MO_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::MO, '%', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case SJ_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::SJ, '^', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case YU_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::YU, '&', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case HUO_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::HUO, '|', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case FAN_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::FAN, '~', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case FXG_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::FXG, '\\', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case SYH_STATE: {
				std::stringstream ss;
				if (!current_char.has_value()) {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidString));
				}
				while (current_char.has_value()&&current_char.value()!='\"') {
					if (!current_char.has_value()) {
						return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidString));
					}
					if (current_char.value() == '\\') {
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						uint8_t tmp;
						switch (current_char.value()) {
						case 'n':
							tmp = '\n';
							ss << tmp;
							break;
						case '\\':
							tmp = '\\';
							ss << tmp;
							break;
						case '\'':
							tmp = '\'';
							ss << tmp;
							break;
						case '\"':
							tmp = '\"';
							ss << tmp;
							break;
						case 'r':
							tmp = '\r';
							ss << tmp;
							break;
						case 't':
							tmp = '\t';
							ss << tmp;
							break;
						case 'x':
							tmp = 0;
							current_char = nextChar();
							if (!current_char.has_value()) {
								return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
							}
							if (current_char.value() >= '0' || current_char.value() <= '9') {
								tmp = tmp * 16 + current_char.value() - '0';
							}
							else if (current_char.value() >= 'a' || current_char.value() <= 'f') {
								tmp = tmp * 16 + current_char.value() - 'a' + 10;
							}
							else if (current_char.value() >= 'A' || current_char.value() <= 'F') {
								tmp = tmp * 16 + current_char.value() - 'A' + 10;
							}
							else {
								return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
							}
							current_char = nextChar();
							if (!current_char.has_value()) {
								return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
							}
							if (current_char.value() >= '0' || current_char.value() <= '9') {
								tmp = tmp * 16 + current_char.value() - '0';
							}
							else if (current_char.value() >= 'a' || current_char.value() <= 'f') {
								tmp = tmp * 16 + current_char.value() - 'a' + 10;
							}
							else if (current_char.value() >= 'A' || current_char.value() <= 'F') {
								tmp = tmp * 16 + current_char.value() - 'A' + 10;
							}
							else {
								return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
							}
							ss << tmp;
							break;
						default:
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
							break;
						}
					}
					else {
						switch (current_char.value()) {
						case '\"':
						case '\\':
						case '\n':
						case '\r':
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						default:
							uint8_t tmp = current_char.value();
							ss << tmp;
							break;
						}
					}
					current_char = nextChar();
				}
				if (!current_char.has_value()) {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidString));
				}
				if (current_char.value() != '\"')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidString));
				return std::make_pair(std::make_optional<Token>(TokenType::STRINGLITER, ss.str(), pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case YH_STATE: {
				
				if (!current_char.has_value()) {
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
				}
				if (current_char.value() == '\\') {
					current_char = nextChar();
					if (!current_char.has_value()) {
						return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
					}
					uint8_t tmp;
					switch (current_char.value()) {
					case 'n':
						tmp = '\n';
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if(current_char.value()!='\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					case '\\':
						tmp = '\\';
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					case '\'':
						tmp = '\'';
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					case '\"':
						tmp = '\"';
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					case 'r':
						tmp = '\r';
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					case 't':
						tmp = '\t';
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					case 'x':
						tmp = 0;
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() >= '0' || current_char.value() <= '9') {
							tmp = tmp * 16 + current_char.value() - '0';
						}
						else if (current_char.value() >= 'a' || current_char.value() <= 'f') {
							tmp = tmp * 16 + current_char.value() - 'a'+10;
						}
						else if (current_char.value() >= 'A' || current_char.value() <= 'F') {
							tmp = tmp * 16 + current_char.value() - 'A' + 10;
						}
						else {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() >= '0' || current_char.value() <= '9') {
							tmp = tmp * 16 + current_char.value() - '0';
						}
						else if (current_char.value() >= 'a' || current_char.value() <= 'f') {
							tmp = tmp * 16 + current_char.value() - 'a' + 10;
						}
						else if (current_char.value() >= 'A' || current_char.value() <= 'F') {
							tmp = tmp * 16 + current_char.value() - 'A' + 10;
						}
						else {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						current_char = nextChar();
						if (!current_char.has_value()) {
							return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						}
						if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
						break;
					default:
						return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
						break;
					}
				}
				else {
				switch (current_char.value()) {
				case '\'':
				case '\\':
				case '\n':
				case '\r':
					return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
				default:
					uint8_t tmp = current_char.value();
					current_char = nextChar();
					if (!current_char.has_value()) {
						return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
					}
					if (current_char.value() != '\'')return std::make_pair(std::optional<Token>(), std::make_optional<CompilationError>(pos, ErrorCode::ErrInvalidChar));
					return std::make_pair(std::make_optional<Token>(TokenType::CHARLITER, tmp, pos, currentPos()), std::optional<CompilationError>());
					break;
				}
				}
				break;
			}
			case FDYH_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::FDYH, '`', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case DOLLAR_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::DOLLAR, '$', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case JING_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::JING, '#', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			case AT_STATE: {
				
				unreadLast(); // Yes, we unread last char even if it's an EOF.
				return std::make_pair(std::make_optional<Token>(TokenType::AT, '@', pos, currentPos()), std::optional<CompilationError>());
				break;
			}
			
								   // 预料之外的状态，如果执行到了这里，说明程序异常
			default:
				DieAndPrint("unhandled state.");
				break;
			}
		}
		// 预料之外的状态，如果执行到了这里，说明程序异常
		return std::make_pair(std::optional<Token>(), std::optional<CompilationError>());
	}

	std::optional<CompilationError> Tokenizer::checkToken(const Token& t) {
		switch (t.GetType()) {
			case IDENTIFIER: {
				auto val = t.GetValueString();
				if (!c0::isalpha(val[0]))
					return std::make_optional<CompilationError>(t.GetStartPos().first, t.GetStartPos().second, ErrorCode::ErrInvalidIdentifier);
				break;
			}
		default:
			break;
		}
		return {};
	}

	void Tokenizer::readAll() {
		if (_initialized)
			return;
		for (std::string tp; std::getline(_rdr, tp);)
			_lines_buffer.emplace_back(std::move(tp + "\n"));
		_initialized = true;
		_ptr = std::make_pair<int64_t, int64_t>(0, 0);
		return;
	}

	// Note: We allow this function to return a postion which is out of bound according to the design like std::vector::end().
	std::pair<uint64_t, uint64_t> Tokenizer::nextPos() {
		if (_ptr.first >= _lines_buffer.size())
			DieAndPrint("advance after EOF");
		if (_ptr.second == _lines_buffer[_ptr.first].size() - 1)
			return std::make_pair(_ptr.first + 1, 0);
		else
			return std::make_pair(_ptr.first, _ptr.second + 1);
	}

	std::pair<uint64_t, uint64_t> Tokenizer::currentPos() {
		return _ptr;
	}

	std::pair<uint64_t, uint64_t> Tokenizer::previousPos() {
		if (_ptr.first == 0 && _ptr.second == 0)
			DieAndPrint("previous position from beginning");
		if (_ptr.second == 0)
			return std::make_pair(_ptr.first - 1, _lines_buffer[_ptr.first - 1].size() - 1);
		else
			return std::make_pair(_ptr.first, _ptr.second - 1);
	}

	std::optional<char> Tokenizer::nextChar() {
		if (isEOF())
			return {}; // EOF
		auto result = _lines_buffer[_ptr.first][_ptr.second];
		_ptr = nextPos();
		return result;
	}

	bool Tokenizer::isEOF() {
		return _ptr.first >= _lines_buffer.size();
	}

	// Note: Is it evil to unread a buffer?
	void Tokenizer::unreadLast() {
		_ptr = previousPos();
	}
}