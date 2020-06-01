#pragma once

#include "error/error.h"
#include "instruction/instruction.h"
#include "tokenizer/token.h"

#include <vector>
#include <optional>
#include <utility>
#include <map>
#include <cstdint>
#include <cstddef> // for std::size_t
#include <iostream>


namespace c0 {
	enum SymType {
		FLAG,
		FUNT,
		INTT,
		CHART,
		DOUBLET,
		STRINGT,
		VOIDT,
		NOPE,
	};
	enum CaseType {
		CDEFAULT,
		CCASE,
	};
	enum JMPType {
		JSWITCH,
		JLOOP,
	};
	class Sym final {
	public:
		bool isConst;
		SymType type;
		std::string name;
		int32_t offset;
		bool isGlobal;
		Sym() :type(FLAG) {}
		Sym(bool _isConst,SymType _type, std::string _name, int32_t _offset,bool _isGlobal) :isConst(_isConst),type(_type), name(_name), offset(_offset) ,isGlobal(_isGlobal){}
		Sym(const Sym& t) {
			this->type = t.type;
			this->name = t.name;
			this->offset = t.offset;
			this->isConst = t.isConst;
			this->isGlobal = t.isGlobal;
		}
		//Sym& operator=(Sym& t) { return *this; }
	};
	class caseItem final {
	public:
		CaseType type;
		int32_t value;
		int nextcondi;
		int nextin;
		caseItem(CaseType _type, int32_t _value) :type(_type), value(_value) {};
		caseItem(CaseType _type,int32_t _value,int _nextcondi,int _nextin) :type(_type),value(_value),nextcondi(_nextcondi),nextin(_nextin){}
		caseItem(const caseItem& t) :type(t.type), value(t.value), nextcondi(t.nextcondi), nextin(t.nextin) {
		}
	};
	class JmpInsT final {
	public:
		JMPType type;
		std::vector<int> breakl;
		std::vector<int> continuel;
		JmpInsT(JMPType _type) :type(_type){};
	};
	// i2,i3,i4的内容，以大端序（big-endian）写入文件
	typedef int8_t  i1;
	typedef int16_t i2;
	typedef int32_t i4;
	typedef int64_t i8;

	// u2,u3,u4的内容，以大端序（big-endian）写入文件
	typedef uint8_t  u1;
	typedef uint16_t u2;
	typedef uint32_t u4;
	typedef uint64_t u8;
	class String_info final{
	public:
		u2 length;
		std::string value;
		bool operator==(const String_info& rhs) const {
			return length == rhs.length
				&& value==rhs.value;
		}
		String_info():value(""),length(0){}
		String_info(std::string v) {
			value = v;
			length = v.length();
		}
	};

	class Int_info final{
	public:
		i4 value;
		bool operator==(const Int_info& rhs) const {
			return value == rhs.value;
		}
		Int_info():value(0){}
		Int_info(std::int32_t t) {
			value = t;
		}
	};

	class Double_info final{
	public:
		double value;
		bool operator==(const Double_info& rhs) const {
			return value == rhs.value;
		}
		Double_info():value(0){}
		Double_info(double t) {
			value = t;
		}
	};

	class Constant_info final{
	public:
		// STRING = 0,
		// INT = 1,
		// DOUBLE = 2
		SymType type;
		// 根据type决定是String_info 还是 Int_info 还是 Double_info
		std::any value;

		Constant_info(SymType _type, std::any _value) :type(_type), value(_value) {}
		Constant_info(const Constant_info& t) {
			this->type = t.type;
			this->value = t.value;
		}
	};

	//struct Instruction {
	//	u1 opcode;
	//	u1 operands[/* size depends on opcode */];
	//};

	class Function_info final{
	public:
		u2          name_index; // name: CO_binary_file.strings[name_index]
		u2          params_size;
		u2          level;
		u2          instructions_count;
		std::vector<Instruction> instructions;
		u2          index;

		std::vector<Sym> params;
		std::string name;
		SymType		returntype;
	};

	class Start_code_info final{
	public:
		u2          instructions_count;
		std::vector<Instruction> instructions;
	};

	class C0_file final{
	public:
		u2              constants_count;
		std::vector<Constant_info>  constants;
		Start_code_info start_code;
		u2              functions_count;
		std::vector<Function_info> functions;
		void genbinary(std::ostream& out);
	};

	
	
	

	class Analyser final {
	private:
		using uint64_t = std::uint64_t;
		using int64_t = std::int64_t;
		using uint32_t = std::uint32_t;
		using int32_t = std::int32_t;
	public:
		Analyser(std::vector<Token> v)
			: _tokens(std::move(v)), _offset(0), _this({}), _current_pos(0, 0),
			_uninitialized_vars({}), _vars({}), _consts({}), _nextTokenIndex(0) {}
		Analyser(Analyser&&) = delete;
		Analyser(const Analyser&) = delete;
		Analyser& operator=(Analyser) = delete;

		// 唯一接口
		std::pair<C0_file, std::optional<CompilationError>> Analyse();
	private:
		// 所有的递归子程序


		std::optional<CompilationError> C0_program();
		std::optional<CompilationError> variable_declaration();
		std::optional<CompilationError> init_declarator_list();
		std::optional<CompilationError> init_declarator();
		std::optional<CompilationError> initializer();
		std::optional<CompilationError> function_definition();
		std::optional<CompilationError> parameter_clause();
		std::optional<CompilationError> parameter_declaration_list();
		std::optional<CompilationError> parameter_declaration();
		std::optional<CompilationError> compound_statement();
		std::optional<CompilationError> statement_seq();
		std::optional<CompilationError> statement();
		std::optional<CompilationError> condition(int* insjmp);
		std::optional<CompilationError> condition_statement();
		std::optional<CompilationError> labeled_statement(std::vector<caseItem>& switchtable);
		std::optional<CompilationError> loop_statement();
		std::optional<CompilationError> for_init_statement();
		std::optional<CompilationError> for_update_expression();
		std::optional<CompilationError> jump_statement();
		std::optional<CompilationError> return_statement();
		std::optional<CompilationError> scan_statement();
		std::optional<CompilationError> print_statement();
		std::optional<CompilationError> printable_list();
		std::optional<CompilationError> printable();
		std::optional<CompilationError> assignment_expression();
		std::optional<CompilationError> expression(SymType* etp);
		std::optional<CompilationError> additive_expression(SymType* etp);
		std::optional<CompilationError> multiplicative_expression(SymType* etp);
		std::optional<CompilationError> cast_expression(SymType* etp);
		std::optional<CompilationError> unary_expression(SymType* etp);
		std::optional<CompilationError> primary_expression(SymType* etp);
		std::optional<CompilationError> function_call(SymType* etp);
		std::optional<CompilationError> expression_list();
		SymType t2t(Token t);
		// Token 缓冲区相关操作

		// 返回下一个 token
		std::optional<Token> nextToken();
		// 回退一个 token
		void unreadToken();

		std::vector<std::optional<Token> > peek(int t);

		// 下面是符号表相关操作
		int32_t getTypesize(SymType type);
		std::optional<Sym> globalVal_find(std::string name);
		std::optional<Sym> globalVal_add(bool isConst, SymType type, std::string name);
		std::optional<Sym> val_findinsym(std::vector<Sym>& t, std::string name);
		std::optional<Sym> val_findinall(std::string name);
		std::optional<CompilationError> infunhead(SymType type, std::string name);
		void infun();
		void outfun();
		void val_inblock();
		void val_outblock();
		std::optional<Sym> val_addinsym(std::vector<Sym>& t, bool isConst, SymType type, std::string name,int32_t offsetas);
		std::optional<Sym> val_add(bool isConst, SymType type, std::string name);
		uint16_t const_getindex_(SymType type, std::any value);
		uint16_t const_getindex(SymType type, std::any value);
		void pusht(Token t);
		void pusht(TokenType t);
		bool popt();
		std::optional<Token> topt();
		std::optional<Function_info> findfun(std::string name);
		int gencode(Operation opr, int32_t p1, int32_t p2);
		int gencode(Operation opr, int32_t p1);
		int gencode(Operation opr);
		int gencode();
		void incodeBuffer();
		std::vector<Instruction>* outcodeBuffer();
		void addbuffer(std::vector<Instruction>* buffer);
		uint16_t nextcodeOffset();
		Instruction* getinsref(int i);
		
	private:
		std::vector<Token> _tokens;
		std::size_t _offset;
		//std::vector<Instruction> _instructions;
		std::pair<uint64_t, uint64_t> _current_pos;

		// 为了简单处理，我们直接把符号表耦合在语法分析里
		// 变量                   示例
		// _uninitialized_vars    var a;
		// _vars                  var a=1;
		// _consts                const a=1;

		std::map<std::string, int32_t> _uninitialized_vars;
		std::map<std::string, int32_t> _vars;
		std::map<std::string, int32_t> _consts;
		// 下一个 token 在栈的偏移
		int32_t _nextTokenIndex;

		//c0
		C0_file _this;
		std::vector<std::vector<Sym>> _syms;
		std::vector<Sym> _parsyms;
		std::vector<Sym> _globalsyms;


		Function_info* _curfun;
		std::vector<Token> _ttmp;


		int _bufferstart;
		std::vector<JmpInsT> jmpstack;
		bool hasMain;
	};
}
