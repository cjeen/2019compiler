#include "fmt/core.h"
#include "tokenizer/tokenizer.h"
#include "analyser/analyser.h"

namespace fmt {
	std::string mem2str(unsigned char* buff, int buff_len)
	{
		char* str = (char*)malloc(buff_len * 2 + 1);
		if (str != NULL) {
			memset(str, 0, buff_len *2 + 1);
			for (int i = 0; i < buff_len; i++) {
				sprintf(str + i*2, "%.2x", buff[buff_len-1-i]);
			}
			std::string res(str);
			//free(str);
			return str;
		}
	}
	template<>
	struct formatter<c0::ErrorCode> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::ErrorCode &p, FormatContext &ctx) {
			std::string name;
			switch (p) {
			case c0::ErrNoError:
				name = "No error.";
				break;
			case c0::ErrStreamError:
				name = "Stream error.";
				break;
			case c0::ErrEOF:
				name = "EOF";
				break;
			case c0::ErrInvalidInput:
				name = "The input is invalid.";
				break;
			case c0::ErrInvalidIdentifier:
				name = "Identifier is invalid";
				break;
			case c0::ErrIntegerOverflow:
				name = "The integer is too big(int64_t).";
				break;
			case c0::ErrNoBegin:
				name = "The program should start with 'begin'.";
				break;
			case c0::ErrNoEnd:
				name = "The program should end with 'end'.";
				break;
			case c0::ErrNeedIdentifier:
				name = "Need an identifier here.";
				break;
			case c0::ErrConstantNeedValue:
				name = "The constant need a value to initialize.";
				break;
			case c0::ErrNoSemicolon:
				name = "Zai? Wei shen me bu xie fen hao.";
				break;
			case c0::ErrInvalidVariableDeclaration:
				name = "The declaration is invalid.";
				break;
			case c0::ErrIncompleteExpression:
				name = "The expression is incomplete.";
				break;
			case c0::ErrNotDeclared:
				name = "The variable or constant must be declared before being used.";
				break;
			case c0::ErrAssignToConstant:
				name = "Trying to assign value to a constant.";
				break;
			case c0::ErrDuplicateDeclaration:
				name = "The variable or constant has been declared.";
				break;
			case c0::ErrNotInitialized:
				name = "The variable has not been initialized.";
				break;
			case c0::ErrInvalidAssignment:
				name = "The assignment statement is invalid.";
				break;
			case c0::ErrInvalidPrint:
				name = "The output statement is invalid.";
				break;
			case c0::ErrInvalidInteger:
				name = "The integer is invalid.";
				break;
			case c0::ErrInvalidComment:
				name = "The comment statement is invalid.";
				break;
			case c0::ErrInvalidChar:
				name = "The char is invalid.";
				break;
			case c0::ErrInvalidString:
				name = "The string is invalid.";
				break;
			case c0::ErrInvalidDouble:
				name = "The double is invalid.";
				break;
			}
			return format_to(ctx.out(), name);
		}
	};

	template<>
	struct formatter<c0::CompilationError> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::CompilationError &p, FormatContext &ctx) {
			return format_to(ctx.out(), "Line: {} Column: {} Error: {}", p.GetPos().first, p.GetPos().second, p.GetCode());
		}
	};
}

namespace fmt {
	template<>
	struct formatter<c0::Token> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Token &p, FormatContext &ctx) {
			return format_to(ctx.out(),
				"Line: {} Column: {} Type: {} Value: {}",
				p.GetStartPos().first, p.GetStartPos().second, p.GetType(), p.GetValueString());
		}
	};

	template<>
	struct formatter<c0::TokenType> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::TokenType &p, FormatContext &ctx) {
			std::string name;
			switch (p) {
			case c0::NULL_TOKEN:
				name = "NullToken";
				break;
			case c0::UNSIGNED_INTEGER:
				name = "UNSIGNED_INTEGER";
				break;
			case c0::IDENTIFIER:
				name = "IDENTIFIER";
				break;
			case c0::CHARLITER:
				name = "CHARLITER";
				break;
			case c0::STRINGLITER:
				name = "STRINGLITER";
				break;
			case c0::DOUBLELITER:
				name = "DOUBLELITER";
				break;
			case c0::CONST:
				name = "CONST";
				break;
			case c0::VOID:
				name = "VOID";
				break;
			case c0::INT:
				name = "INT";
				break;
			case c0::CHAR:
				name = "CHAR";
				break;
			case c0::DOUBLE:
				name = "DOUBLE";
				break;
			case c0::STRUCT:
				name = "STRUCT";
				break;
			case c0::IF:
				name = "IF";
				break;
			case c0::ELSE:
				name = "ELSE";
				break;
			case c0::SWITCH:
				name = "SWITCH";
				break;
			case c0::CASE:
				name = "CASE";
				break;
			case c0::DEFAULT:
				name = "DEFAULT";
				break;
			case c0::WHILE:
				name = "WHILE";
				break;
			case c0::FOR:
				name = "FOR";
				break;
			case c0::DO:
				name = "DO";
				break;
			case c0::RETURN:
				name = "RETURN";
				break;
			case c0::BREAK:
				name = "BREAK";
				break;
			case c0::CONTINUE:
				name = "CONTINUE";
				break;
			case c0::PRINT:
				name = "PRINT";
				break;
			case c0::SCAN:
				name = "SCAN";
				break;
			case c0::XHX:
				name = "XHX";
				break;
			case c0::YKHZ:
				name = "YKHZ";
				break;
			case c0::YKHY:
				name = "YKHY";
				break;
			case c0::FKHZ:
				name = "FKHZ";
				break;
			case c0::FKHY:
				name = "FKHY";
				break;
			case c0::HKHZ:
				name = "HKHZ";
				break;
			case c0::HKHY:
				name = "HKHY";
				break;
			case c0::XYH:
				name = "XYH";
				break;
			case c0::DH:
				name = "DH";
				break;
			case c0::DYH:
				name = "DYH";
				break;
			case c0::DIAN:
				name = "DIAN";
				break;
			case c0::DOU:
				name = "DOU";
				break;
			case c0::MH:
				name = "MH";
				break;
			case c0::FH:
				name = "FH";
				break;
			case c0::TH:
				name = "TH";
				break;
			case c0::WH:
				name = "WH";
				break;
			case c0::JIA:
				name = "JIA";
				break;
			case c0::JIAN:
				name = "JIAN";
				break;
			case c0::CHENG:
				name = "CHENG";
				break;
			case c0::CHU:
				name = "CHU";
				break;
			case c0::MO:
				name = "MO";
				break;
			case c0::SJ:
				name = "SJ";
				break;
			case c0::YU:
				name = "YU";
				break;
			case c0::HUO:
				name = "HUO";
				break;
			case c0::FAN:
				name = "FAN";
				break;
			case c0::FXG:
				name = "FXG";
				break;
			case c0::SYH:
				name = "SYH";
				break;
			case c0::YH:
				name = "YH";
				break;
			case c0::FDYH:
				name = "FDYH";
				break;
			case c0::DOLLAR:
				name = "DOLLAR";
				break;
			case c0::JING:
				name = "JING";
				break;
			case c0::AT:
				name = "AT";
				break;
			case c0::XYDY:
				name = "XYDY";
				break;
			case c0::DDY:
				name = "DDY";
				break;
			case c0::DYDY:
				name = "DYDY";
				break;
			case c0::BDY:
				name = "BDY";
				break;
			}
			return format_to(ctx.out(), name);
		}
	};
}

namespace fmt {
	template<>
	struct formatter<c0::Operation> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Operation &p, FormatContext &ctx) {
			std::string name;
			switch (p) {
			case c0::NOP:
				name = "nop";
				break;
			case c0::BIPUSH:
				name = "bipush";
				break;
			case c0::IPUSH:
				name = "ipush";
				break;
			case c0::POP:
				name = "pop";
				break;
			case c0::POP2:
				name = "pop2";
				break;
			case c0::POPN:
				name = "popn";
				break;
			case c0::DUP:
				name = "dup";
				break;
			case c0::DUP2:
				name = "dup2";
				break;
			case c0::LOADC:
				name = "loadc";
				break;
			case c0::LOADA:
				name = "loada";
				break;
			case c0::NEW:
				name = "new";
				break;
			case c0::SNEW:
				name = "snew";
				break;
			case c0::ILOAD:
				name = "iload";
				break;
			case c0::DLOAD:
				name = "dload";
				break;
			case c0::ALOAD:
				name = "aload";
				break;
			case c0::IALOAD:
				name = "iaload";
				break;
			case c0::DALOAD:
				name = "daload";
				break;
			case c0::AALOAD:
				name = "aaload";
				break;
			case c0::ISTORE:
				name = "istore";
				break;
			case c0::DSTORE:
				name = "dstore";
				break;
			case c0::ASTORE:
				name = "astore";
				break;
			case c0::IASTORE:
				name = "iastore";
				break;
			case c0::DASTORE:
				name = "dastore";
				break;
			case c0::AASTORE:
				name = "aastore";
				break;
			case c0::IADD:
				name = "iadd";
				break;
			case c0::DADD:
				name = "dadd";
				break;
			case c0::ISUB:
				name = "isub";
				break;
			case c0::DSUB:
				name = "dsub";
				break;
			case c0::IMUL:
				name = "imul";
				break;
			case c0::DMUL:
				name = "dmul";
				break;
			case c0::IDIV:
				name = "idiv";
				break;
			case c0::DDIV:
				name = "ddiv";
				break;
			case c0::INEG:
				name = "ineg";
				break;
			case c0::DNEG:
				name = "dneg";
				break;
			case c0::ICMP:
				name = "icmp";
				break;
			case c0::DCMP:
				name = "dcmp";
				break;
			case c0::I2D:
				name = "i2d";
				break;
			case c0::D2I:
				name = "d2i";
				break;
			case c0::I2C:
				name = "i2c";
				break;
			case c0::JMP:
				name = "jmp";
				break;
			case c0::JE:
				name = "je";
				break;
			case c0::JNE:
				name = "jne";
				break;
			case c0::JL:
				name = "jl";
				break;
			case c0::JGE:
				name = "jge";
				break;
			case c0::JG:
				name = "jg";
				break;
			case c0::JLE:
				name = "jle";
				break;
			case c0::CALL:
				name = "call";
				break;
			case c0::RET:
				name = "ret";
				break;
			case c0::IRET:
				name = "iret";
				break;
			case c0::DRET:
				name = "dret";
				break;
			case c0::ARET:
				name = "aret";
				break;
			case c0::IPRINT:
				name = "iprint";
				break;
			case c0::DPRINT:
				name = "dprint";
				break;
			case c0::CPRINT:
				name = "cprint";
				break;
			case c0::SPRINT:
				name = "sprint";
				break;
			case c0::PRINTL:
				name = "printl";
				break;
			case c0::ISCAN:
				name = "iscan";
				break;
			case c0::DSCAN:
				name = "dscan";
				break;
			case c0::CSCAN:
				name = "cscan";
				break;
			}
			return format_to(ctx.out(), name);
		}
	};
	template<>
	struct formatter<c0::Instruction> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Instruction &p, FormatContext &ctx) {
			std::string name;
			switch (p.GetOperation())
			{
			case c0::NOP:
			case c0::POP:
			case c0::POP2:
			case c0::DUP:
			case c0::DUP2:
			case c0::NEW:
			case c0::ILOAD:
			case c0::DLOAD:
			case c0::ALOAD:
			case c0::IALOAD:
			case c0::DALOAD:
			case c0::AALOAD:
			case c0::ISTORE:
			case c0::DSTORE:
			case c0::ASTORE:
			case c0::IASTORE:
			case c0::DASTORE:
			case c0::AASTORE:
			case c0::IADD:
			case c0::DADD:
			case c0::ISUB:
			case c0::DSUB:
			case c0::IMUL:
			case c0::DMUL:
			case c0::IDIV:
			case c0::DDIV:
			case c0::INEG:
			case c0::DNEG:
			case c0::ICMP:
			case c0::DCMP:
			case c0::I2D:
			case c0::D2I:
			case c0::I2C:
			case c0::RET:
			case c0::IRET:
			case c0::DRET:
			case c0::ARET:
			case c0::IPRINT:
			case c0::DPRINT:
			case c0::CPRINT:
			case c0::SPRINT:
			case c0::PRINTL:
			case c0::ISCAN:
			case c0::DSCAN:
			case c0::CSCAN:
				return format_to(ctx.out(), "{}", p.GetOperation());
			case c0::BIPUSH:
			case c0::IPUSH:
			case c0::POPN:
			case c0::LOADC:
			case c0::SNEW:
			case c0::JMP:
			case c0::JE:
			case c0::JNE:
			case c0::JL:
			case c0::JGE:
			case c0::JG:
			case c0::JLE:
			case c0::CALL:
				return format_to(ctx.out(), "{} {}", p.GetOperation(), p.GetP1());
			case c0::LOADA:
				return format_to(ctx.out(), "{} {},{}", p.GetOperation(), p.GetP1(), p.GetP2());
			}
			return format_to(ctx.out(), "NOP");
		}
	};
	template<>
	struct formatter<c0::Int_info> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Int_info& p, FormatContext& ctx) {
			return format_to(ctx.out(),
				"{}",
				p.value);
		}
	};
	template<>
	struct formatter<c0::Double_info> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Double_info& p, FormatContext& ctx) {
			return format_to(ctx.out(),
				"0x{}",
				mem2str((unsigned char*)(&p.value), 8));
		}
	};
	template<>
	struct formatter<c0::String_info> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::String_info& p, FormatContext& ctx) {
			return format_to(ctx.out(),
				p.value);
		}
	};
	template<>
	struct formatter<c0::Constant_info> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Constant_info& p, FormatContext& ctx) {
			if (p.type == c0::STRINGT) {
				c0::String_info value;
				try {
					 value=std::any_cast<c0::String_info>(p.value);
				}
				catch (const std::bad_any_cast&) {
					c0::DieAndPrint("No suitable cast.");
				}
				return format_to(ctx.out(),
					"S \"{}\"",
					value);
			}
			else if(p.type == c0::INTT) {
				c0::Int_info value;
				try {
					value = std::any_cast<c0::Int_info>(p.value);
				}
				catch (const std::bad_any_cast&) {
					c0::DieAndPrint("No suitable cast.");
				}
				return format_to(ctx.out(),
					"I {}",
					value);
			}
			else if (p.type == c0::DOUBLET) {
				c0::Double_info value;
				try {
					value = std::any_cast<c0::Double_info>(p.value);
				}
				catch (const std::bad_any_cast&) {
					c0::DieAndPrint("No suitable cast.");
				}
				return format_to(ctx.out(),
					"D {}",
					value);
			}
			else {
				return format_to(ctx.out(),
					"VALID");
			}
			
		}
	};
	template<>
	struct formatter<c0::Function_info> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Function_info& p, FormatContext& ctx) {
			std::stringstream ss;
			for (int i = 0; i < p.instructions.size(); ++i) {
				ss<<i<<'\t'<<fmt::format("{}\n", p.instructions[i]);
			}
			return format_to(ctx.out(),
				ss.str());
		}
	};
	template<>
	struct formatter<c0::Start_code_info> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::Start_code_info& p, FormatContext& ctx) {
			std::stringstream ss;
			for (int i = 0; i < p.instructions.size(); ++i) {
				ss << i << '\t' << fmt::format("{}\n", p.instructions[i]);
			}
			return format_to(ctx.out(),
				ss.str());
		}
	};
	template<>
	struct formatter<c0::C0_file> {
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template <typename FormatContext>
		auto format(const c0::C0_file& p, FormatContext& ctx) {
			std::stringstream ss;
			ss << ".constants:" << '\n';
			for (int i = 0; i < p.constants.size(); ++i) {
				ss << i << ' ' << fmt::format("{}\n", p.constants[i]);
			}
			ss << ".start:" << '\n';
			ss << fmt::format("{}", p.start_code);
			ss << ".functions:" << '\n';
			for (int i = 0; i < p.functions.size(); ++i) {
				ss << i << ' ' << fmt::format("{} {} {}\n", p.functions[i].name_index, p.functions[i].params_size, p.functions[i].level);
			}
			for (int i = 0; i < p.functions.size(); ++i) {
				ss <<".F"<<i<<':'<<'\n';
				ss << fmt::format("{}", p.functions[i]);
			}
			return format_to(ctx.out(),
				ss.str());
		}
	};
}