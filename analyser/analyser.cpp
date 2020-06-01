#include "analyser.h"

#include <climits>

namespace c0 {
	std::pair<C0_file, std::optional<CompilationError>> Analyser::Analyse() {
		hasMain = false;
		_curfun = NULL;
		auto err = C0_program();
		auto nt = nextToken();
		if (err.has_value()) {
			return std::make_pair(C0_file(), err);
		}
		else {
			if (nt.has_value()) {
				return std::make_pair(_this, std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid));
			}
			if(hasMain==false)return std::make_pair(_this, std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid));
			return std::make_pair(_this, std::optional<CompilationError>());
		}	
	}

	/*<C0_program> :: =
	{ <variable_declaration> } {<function_definition>}*/
	std::optional<CompilationError> Analyser::C0_program() {
		//imp{}
		auto p1 = peek(3);
		while (p1.size() == 3 && p1[0].has_value()
			&& p1[1].has_value() 
			&& p1[2].has_value() 
			&& (p1[0].value().GetType() == TokenType::CONST
			|| p1[0].value().GetType() == TokenType::VOID && p1[2].value().GetType() != TokenType::YKHZ
			|| p1[0].value().GetType() == TokenType::INT && p1[2].value().GetType() != TokenType::YKHZ
			|| p1[0].value().GetType() == TokenType::CHAR && p1[2].value().GetType() != TokenType::YKHZ
			|| p1[0].value().GetType() == TokenType::DOUBLE && p1[2].value().GetType() != TokenType::YKHZ)) {
			
			//imp<variable_declaration>
			auto err = variable_declaration();
			if (err.has_value())
				return err;
			p1 = peek(3);
		}
		
		//imp{}
		p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::VOID
			|| p1[0].value().GetType() == TokenType::INT
			|| p1[0].value().GetType() == TokenType::CHAR
			|| p1[0].value().GetType() == TokenType::DOUBLE)) {
			//imp<function_definition>
			auto err = function_definition();
			if (err.has_value())
				return err;
			p1 = peek(1);
		}
		return {};
	}

	/*<variable_declaration> :: =
		[<const_qualifier>]<type_specifier> < init_declarator_list>';'*/
	std::optional<CompilationError> Analyser::variable_declaration() {
		int cnt = 0;
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::CONST)) {
		//imp<const_qualifier>
			auto cq = nextToken();
			if (!cq.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (cq.value().GetType() == TokenType::CONST) {
				
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			pusht(cq.value());
			cnt++;
		}
		//imp<type_specifier>
		auto ts = nextToken();
		if (!ts.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (ts.value().GetType() == TokenType::VOID) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		else if (ts.value().GetType() == TokenType::INT) {

		}
		else if (ts.value().GetType() == TokenType::CHAR) {

		}
		else if (ts.value().GetType() == TokenType::DOUBLE) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		pusht(ts.value());
		cnt++;
		//imp< init_declarator_list>
		auto err = init_declarator_list();
		if (err.has_value())
			return err;
		//imp';'
		auto fh = nextToken();
		if (!fh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (fh.value().GetType() == TokenType::FH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}

		//pop
		while (cnt--) {
			popt();
		}
		return {};
	}
	/*<init_declarator_list> :: =
		<init_declarator>{ ',' < init_declarator > }*/
	std::optional<CompilationError> Analyser::init_declarator_list() {
		//imp<init_declarator>
		auto err = init_declarator();
		if (err.has_value())
			return err;

		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DOU)) {
			//imp','
			auto dh = nextToken();
			if (!dh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (dh.value().GetType() == TokenType::DOU) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< init_declarator >
			auto err = init_declarator();
			if (err.has_value())
				return err;


			p1 = peek(1);
		}

		return {};
	}
	/*<init_declarator> :: =
		<identifier>[<initializer>]*/
	std::optional<CompilationError> Analyser::init_declarator() {
		bool isConst = false;
		int ini = false;
		//imp<identifier>
		auto id = nextToken();
		SymType tp;
		if (!id.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (id.value().GetType() == TokenType::IDENTIFIER) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		if (_ttmp[0].GetType() == CONST) {
			isConst = true;
			if (_ttmp[1].GetType() == CHAR) {
				tp = CHART;
				auto res=val_add(true, CHART, id.value().GetValueString());
				if (!res.has_value()) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (_ttmp[1].GetType() == INT) {
				tp = INTT;
				auto res=val_add(true, INTT, id.value().GetValueString());
				if (!res.has_value()) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (_ttmp[1].GetType() == VOID) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			else if (_ttmp[1].GetType() == DOUBLE) {
				tp = DOUBLET;
				auto res=val_add(true, DOUBLET, id.value().GetValueString());
				if (!res.has_value()) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		else {
			if (_ttmp[0].GetType() == CHAR) {
				tp = CHART;
				auto res=val_add(false, CHART, id.value().GetValueString());
				if (!res.has_value()) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (_ttmp[0].GetType() == INT) {
				tp = INTT;
				auto res=val_add(false, INTT, id.value().GetValueString());
				if (!res.has_value()) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (_ttmp[0].GetType() == VOID) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			else if (_ttmp[0].GetType() == DOUBLE) {
				tp = DOUBLET;
				auto res=val_add(false, DOUBLET, id.value().GetValueString());
				if (!res.has_value()) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		pusht(id.value());
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DH)) {
			//imp<initializer>
			auto err = initializer();
			if (err.has_value())
				return err;
			ini = true;
		}

		popt();


		//gencode
		if (ini == true) {
			//todo:???nothing
		}
		else {
			if (isConst == true) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrConstantNeedValue);
			}
			if (tp == INTT) {
				gencode(IPUSH, 0);
			}
			else if (tp == DOUBLET) {
				gencode(IPUSH, 0);
				gencode(IPUSH, 0);
			}
			else if (tp == CHART) {
				gencode(BIPUSH, 0);
			}
			else {
				DieAndPrint("shouldn't be here3.");
			}
		}
		return {};
	}
	/*<initializer> :: =
		'=' < expression >*/
	std::optional<CompilationError> Analyser::initializer() {
		TokenType task;
		//imp'='
		auto dh = nextToken();
		if (!dh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (dh.value().GetType() == TokenType::DH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp< expression >
		SymType rtp=NOPE;
		SymType ltp=NOPE;
		auto err = expression(&rtp);
		if (err.has_value())
			return err;
		if (_ttmp[0].GetType() == CONST) {
			ltp = t2t(_ttmp[1]);
		}
		else{
			ltp = t2t(_ttmp[0]);
		}
		if (rtp != ltp) {
			if (ltp == INTT) {
				if (rtp == DOUBLET) {
					gencode(D2I);
				}
				else if (rtp == CHART) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (ltp == DOUBLET) {
				if (rtp == INTT) {
					gencode(I2D);
				}
				else if (rtp == CHART) {
					gencode(I2D);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (ltp == CHART) {
				if (rtp == DOUBLET) {
					gencode(D2I);
					gencode(I2C);
				}
				else if (rtp == INTT) {
					gencode(I2C);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		return {};
	}
	/*<function_definition> :: =
		<type_specifier><identifier><parameter_clause><compound_statement>*/
	std::optional<CompilationError> Analyser::function_definition() {
		SymType tp;
		std::string fname;
		//imp<type_specifier>
		auto ts = nextToken();
		if (!ts.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (ts.value().GetType() == TokenType::VOID) {
			tp = VOIDT;
		}
		else if (ts.value().GetType() == TokenType::INT) {
			tp = INTT;
		}
		else if (ts.value().GetType() == TokenType::CHAR) {
			tp = CHART;
		}
		else if (ts.value().GetType() == TokenType::DOUBLE) {
			tp = DOUBLET;
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp<identifier>
		auto id = nextToken();
		if (!id.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (id.value().GetType() == TokenType::IDENTIFIER) {
			fname = id.value().GetValueString();
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}

		auto errin=infunhead(tp, fname);
		if (fname == "main")hasMain = true;
		if (errin.has_value()) {
			return errin;
		}
		//imp<parameter_clause>

		auto err = parameter_clause();
		if (err.has_value())
			return err;
		infun();
		//imp<compound_statement>
		
		err = compound_statement();
		if (err.has_value())
			return err;
		if (tp == VOIDT) {
			gencode(RET);
		}
		else if (tp == INTT) {
			gencode(IPUSH, 0);
			gencode(IRET);
		}
		else if (tp == CHART) {
			gencode(BIPUSH, 0);
			gencode(IRET);
		}
		else if (tp == DOUBLET) {
			gencode(IPUSH, 0);
			gencode(IPUSH, 0);
			gencode(DRET);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		outfun();
		return {};
	}
	/*<parameter_clause> :: =
		'('[<parameter_declaration_list>] ')'*/
	std::optional<CompilationError> Analyser::parameter_clause() {
		//imp'('
		auto khz = nextToken();
		if (!khz.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khz.value().GetType() == TokenType::YKHZ) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::CONST
			|| p1[0].value().GetType() == TokenType::VOID
			|| p1[0].value().GetType() == TokenType::INT
			|| p1[0].value().GetType() == TokenType::CHAR
			|| p1[0].value().GetType() == TokenType::DOUBLE)) {
			//imp<parameter_declaration_list>
			auto err = parameter_declaration_list();
			if (err.has_value())
				return err;
		}
		//imp')'
		auto khy = nextToken();
		if (!khy.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khy.value().GetType() == TokenType::YKHY) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}

		return {};
	}
	/*<parameter_declaration_list> :: =
		<parameter_declaration>{ ',' < parameter_declaration > }*/
	std::optional<CompilationError> Analyser::parameter_declaration_list() {
		//imp<parameter_declaration>
		auto err = parameter_declaration();
		if (err.has_value())
			return err;
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DOU)) {
			//imp','
			auto dh = nextToken();
			if (!dh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (dh.value().GetType() == TokenType::DOU) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< parameter_declaration >
			auto err = parameter_declaration();
			if (err.has_value())
				return err;

			p1 = peek(1);
		}
		return {};
	}
	/*<parameter_declaration> :: =
		[<const_qualifier>]<type_specifier><identifier>*/
	std::optional<CompilationError> Analyser::parameter_declaration() {
		bool isConst = false;
		SymType tp;
		std::string pname;
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::CONST)) {
			//imp<const_qualifier>
			auto cq = nextToken();
			if (!cq.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (cq.value().GetType() == TokenType::CONST) {
				isConst = true;
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		//imp<type_specifier>
		auto ts = nextToken();
		if (!ts.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (ts.value().GetType() == TokenType::VOID) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		else if (ts.value().GetType() == TokenType::INT) {
			tp = INTT;
		}
		else if (ts.value().GetType() == TokenType::CHAR) {
			tp = CHART;
		}
		else if (ts.value().GetType() == TokenType::DOUBLE) {
			tp = DOUBLET;
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp<identifier>
		auto id = nextToken();
		if (!id.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (id.value().GetType() == TokenType::IDENTIFIER) {
			pname = id.value().GetValueString();
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto res=val_add(isConst, tp, pname);
		if (!res.has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//todo:???gencode nothing
		

		return {};
	}
	/*<compound_statement> :: =
		'{' {<variable_declaration>} < statement_seq> '}'*/
	std::optional<CompilationError> Analyser::compound_statement() {
		//imp'{'
		auto khz = nextToken();
		if (!khz.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khz.value().GetType() == TokenType::HKHZ) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		val_inblock();
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::CONST
			|| p1[0].value().GetType() == TokenType::VOID
			|| p1[0].value().GetType() == TokenType::INT
			|| p1[0].value().GetType() == TokenType::CHAR
			|| p1[0].value().GetType() == TokenType::DOUBLE)) {
			//imp<variable_declaration>
			auto err = variable_declaration();
			if (err.has_value())
				return err;
			p1 = peek(1);
		}
		//imp< statement_seq>
		auto err = statement_seq();
		if (err.has_value())
			return err;
		//imp'}'
		auto khy = nextToken();
		if (!khy.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khy.value().GetType() == TokenType::HKHY) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		val_outblock();
		return {};
	}
	/*<statement_seq> :: =
	{ <statement> }*/
	std::optional<CompilationError> Analyser::statement_seq() {
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::HKHZ ||
			p1[0].value().GetType() == TokenType::IF ||
			p1[0].value().GetType() == TokenType::SWITCH ||
			p1[0].value().GetType() == TokenType::WHILE ||
			p1[0].value().GetType() == TokenType::DO ||
			p1[0].value().GetType() == TokenType::FOR ||
			p1[0].value().GetType() == TokenType::BREAK ||
			p1[0].value().GetType() == TokenType::CONTINUE ||
			p1[0].value().GetType() == TokenType::RETURN ||
			p1[0].value().GetType() == TokenType::PRINT ||
			p1[0].value().GetType() == TokenType::SCAN ||
			p1[0].value().GetType() == TokenType::IDENTIFIER)) {
			//imp<statement>
			auto err = statement();
			if (err.has_value())
				return err;
			p1 = peek(1);
		}
		return {};
	}
	//'{' if switch while do for 'break'||'continue'||'return' 'print' 'scan' <identifier> ';'
	/*<statement> :: =
		<compound_statement>
		| <condition_statement>
		| <loop_statement>
		| <jump_statement>
		| <print_statement>
		| <scan_statement>
		| < assignment_expression>';'
		| < function_call>';'
		| ';'*/
	std::optional<CompilationError> Analyser::statement() {
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1 || !p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		if (p1v.GetType() == TokenType::HKHZ) {
			//imp<compound_statement>
			auto err = compound_statement();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == TokenType::IF
			|| p1v.GetType() == TokenType::SWITCH) {
			//imp<condition_statement>
			auto err = condition_statement();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == TokenType::WHILE
			|| p1v.GetType() == TokenType::DO
			|| p1v.GetType() == TokenType::FOR) {
			//imp<loop_statement>
			auto err = loop_statement();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == TokenType::BREAK
			|| p1v.GetType() == TokenType::CONTINUE
			|| p1v.GetType() == TokenType::RETURN) {
			//imp<jump_statement>
			auto err = jump_statement();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == TokenType::PRINT) {
			//imp<print_statement>
			auto err = print_statement();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == TokenType::SCAN) {
			//imp<scan_statement>
			auto err = scan_statement();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == TokenType::IDENTIFIER) {
			//imp< assignment_expression>|< function_call>
			auto p2 = peek(2);
			if (p2.size() != 2 || !p2[0].has_value() || !p2[1].has_value() ) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			auto p2v = p2[1].value();
			if (p2v.GetType() == DH) {
				//imp< assignment_expression>
				auto err = assignment_expression();
				if (err.has_value())
					return err;
				//imp';'
				auto fh = nextToken();
				if (!fh.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (fh.value().GetType() == TokenType::FH) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (p2v.GetType() == YKHZ) {
				SymType tmp;
				//imp< function_call>
				auto err = function_call(&tmp);
				if (err.has_value())
					return err;
				//imp';'
				auto fh = nextToken();
				if (!fh.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (fh.value().GetType() == TokenType::FH) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
				if (tmp == VOIDT) {

				}
				else if (tmp == DOUBLET) {
					gencode(POP2);
				}
				else if (tmp == INTT) {
					gencode(POP);
				}
				else if (tmp == CHART) {
					gencode(POP);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		else if (p1v.GetType() == TokenType::FH) {
			//imp';'
			auto fh = nextToken();
			if (!fh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (fh.value().GetType() == TokenType::FH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<condition> :: =
		<expression>[<relational_operator><expression>]*/
	std::optional<CompilationError> Analyser::condition(int* insjmp) {
		bool isSingle = true;
		//imp<expression>
		SymType rtp=NOPE;
		SymType ltp=NOPE;
		SymType restp = NOPE;
		auto err = expression(&ltp);
		if (err.has_value())
			return err;
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::XYH
			|| p1[0].value().GetType() == TokenType::XYDY
			|| p1[0].value().GetType() == TokenType::DYH
			|| p1[0].value().GetType() == TokenType::DYDY
			|| p1[0].value().GetType() == TokenType::DDY
			|| p1[0].value().GetType() == TokenType::BDY)) {
			//imp<relational_operator>
			auto ro = nextToken();
			if (!ro.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (ro.value().GetType() == TokenType::XYH) {

			}
			else if (ro.value().GetType() == TokenType::XYDY) {

			}
			else if (ro.value().GetType() == TokenType::DYH) {

			}
			else if (ro.value().GetType() == TokenType::DYDY){

			}
			else if (ro.value().GetType() == TokenType::DDY) {

			}
			else if (ro.value().GetType() == TokenType::BDY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp<expression>
			auto tmpf = *_curfun;
			auto tmpp = _offset;
			SymType tmptp;
			auto err = expression(&rtp);
			if (err.has_value())
				return err;
			//todo:gencode
			if (ltp != rtp) {
				if (ltp == INTT) {
					if (rtp == DOUBLET) {
						*_curfun = tmpf;
						_offset = tmpp;
						gencode(I2D);
						auto err = expression(&tmptp);
						if (err.has_value())
							return err;
						restp = DOUBLET;
					}
					else if (rtp == CHART) {
						restp = INTT;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ltp == DOUBLET) {
					if (rtp == INTT) {
						gencode(I2D);
						restp = DOUBLET;
					}
					else if (rtp == CHART) {
						gencode(I2D);
						restp = DOUBLET;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ltp == CHART) {
					if (rtp == DOUBLET) {
						*_curfun = tmpf;
						_offset = tmpp;
						gencode(I2D);
						auto err = expression(&tmptp);
						if (err.has_value())
							return err;
						restp = DOUBLET;
					}
					else if (rtp == INTT) {
						restp = INTT;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				restp = ltp;
				if (restp == CHART)restp = INTT;
			}
			isSingle = false;
			if (restp == CHART) {
				gencode(ICMP);
			}
			else if (restp == INTT) {
				gencode(ICMP);
			}
			else if(restp == DOUBLET) {
				gencode(DCMP);
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			if (ro.value().GetType() == TokenType::XYH) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JGE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JL, 0);
				}
			}
			else if (ro.value().GetType() == TokenType::XYDY) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JG, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JLE, 0);
				}
			}
			else if (ro.value().GetType() == TokenType::DYH) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JLE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JG, 0);
				}
			}
			else if (ro.value().GetType() == TokenType::DYDY) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JL, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JGE, 0);
				}
			}
			else if (ro.value().GetType() == TokenType::DDY) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JNE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JE, 0);
				}
			}
			else if (ro.value().GetType() == TokenType::BDY) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JNE, 0);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		if (isSingle) {
			if (ltp == CHART) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JNE, 0);
				}
			}
			else if (ltp == INTT) {
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JNE, 0);
				}
			}
			else if (ltp == DOUBLET) {
				gencode(D2I);
				if (topt().value().GetType() == IFC
					|| topt().value().GetType() == WHILEC
					|| topt().value().GetType() == FORC) {
					*insjmp = gencode(JE, 0);
				}
				else if (topt().value().GetType() == DOWHILEC) {
					*insjmp = gencode(JNE, 0);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		
		return {};
	}
	//'if'||'switch'
	/*<condition_statement> :: =
		'if' '(' < condition > ')' < statement > ['else' < statement > ]
		| 'switch' '(' < expression > ')' '{' {<labeled_statement>} '}'*/
	std::optional<CompilationError> Analyser::condition_statement() {
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1 || !p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		if (p1v.GetType() == TokenType::IF) {
			//imp 'if'
			auto ifh = nextToken();
			if (!ifh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (ifh.value().GetType() == TokenType::IF) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< condition >
			pusht(IFC);
			int insjmp;
			auto err = condition(&insjmp);
			if (err.has_value())
				return err;
			popt();
			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			
			//imp< statement >
			err = statement();
			if (err.has_value())
				return err;
			int jend=gencode(JMP, 0);
			getinsref(insjmp)->SetP(nextcodeOffset());
			getinsref(jend)->SetP(nextcodeOffset());
			//imp[]
			auto p1 = peek(1);
			if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::ELSE)) {
				//imp 'else'
				auto elseh = nextToken();
				if (!elseh.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (elseh.value().GetType() == TokenType::ELSE) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
				
				
				//imp < statement >
				auto err = statement();
				if (err.has_value())
					return err;
				getinsref(jend)->SetP(nextcodeOffset());
			}
		}
		else if (p1v.GetType() == TokenType::SWITCH) {
			std::vector<caseItem> switchtable;
			//imp 'switch'
			auto switchh = nextToken();
			if (!switchh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (switchh.value().GetType() == TokenType::SWITCH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< expression > 
			SymType etp;
			auto err = expression(&etp);
			if (err.has_value())
				return err;
			if (etp != INTT&&etp!=CHART) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}

			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp'{'
			auto hkhz = nextToken();
			if (!hkhz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (hkhz.value().GetType() == TokenType::HKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			val_inblock();
			jmpstack.emplace_back(JSWITCH);
			//imp{}
			auto p1 = peek(1);
			while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::CASE
				|| p1[0].value().GetType() == TokenType::DEFAULT)) {
				//imp<labeled_statement>
				auto err = labeled_statement(switchtable);
				if (err.has_value())
					return err;
				p1 = peek(1);
			}
			//imp'}'
			auto hkhy = nextToken();
			if (!hkhy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (hkhy.value().GetType() == TokenType::HKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			val_outblock();
			for (auto& v: jmpstack.back().breakl) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			jmpstack.pop_back();
			if (switchtable.back().type==CCASE) {
				getinsref(switchtable.back().nextcondi)->SetP(nextcodeOffset());
				getinsref(switchtable.back().nextin)->SetP(nextcodeOffset());
			}
			else if(switchtable.back().type == CDEFAULT){
				getinsref(switchtable.back().nextin)->SetP(nextcodeOffset());
			}
			else {
				//todo
			}
			gencode(POP);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//'case'||'default'
	/*<labeled_statement> :: =
		'case' (<integer_literal> | <char_literal>) ':' < statement >
		| 'default' ':' < statement >*/
	std::optional<CompilationError> Analyser::labeled_statement(std::vector<caseItem>& switchtable) {
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1 || !p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		if (p1v.GetType() == TokenType::CASE) {
			if (switchtable.size() > 0&&switchtable.back().type == CDEFAULT) {
				//UB
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			if (switchtable.size() > 0) {
				getinsref(switchtable.back().nextcondi)->SetP(nextcodeOffset());
			}
			gencode(DUP);
			//imp 'case'
			auto caseh = nextToken();
			if (!caseh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (caseh.value().GetType() == TokenType::CASE) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}

			//imp |
			auto p1 = peek(1);
			if (p1.size() != 1 || !p1[0].has_value()) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			auto p1v = p1[0].value();
			if (p1v.GetType() == TokenType::UNSIGNED_INTEGER) {
				//imp<integer_literal>
				auto il = nextToken();
				if (!il.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (il.value().GetType() == TokenType::UNSIGNED_INTEGER) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}


				for (auto& v : switchtable) {
					if (v.type == CCASE&&v.value==std::any_cast<int32_t>(il.value().GetValue())){
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				gencode(LOADC, const_getindex(INTT, il.value().GetValue()));
				gencode(ICMP);
				switchtable.emplace_back(CCASE, std::any_cast<int32_t>(il.value().GetValue()));
				switchtable.back().nextcondi = gencode(JNE,0);
			}
			else if (p1v.GetType() == TokenType::CHARLITER) {
				//imp<char_literal>
				auto cl = nextToken();
				if (!cl.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (cl.value().GetType() == TokenType::CHARLITER) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}


				for (auto& v : switchtable) {
					if (v.type == CCASE && v.value == std::any_cast<uint8_t>(cl.value().GetValue())) {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				gencode(LOADC, const_getindex(CHART, cl.value().GetValue()));
				gencode(ICMP);
				switchtable.emplace_back(CCASE, std::any_cast<uint8_t>(cl.value().GetValue()));
				switchtable.back().nextcondi = gencode(JNE, 0);
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp ':'
			auto mh = nextToken();
			if (!mh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (mh.value().GetType() == TokenType::MH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp < statement >
			if (switchtable.size() > 1) {
				getinsref(switchtable[switchtable.size()-2].nextin)->SetP(nextcodeOffset());
			}
			auto err = statement();
			if (err.has_value())
				return err;
			switchtable.back().nextin = gencode(JMP, 0);
		}
		else if (p1v.GetType() == TokenType::DEFAULT) {
			if (switchtable.size() > 0) {
				getinsref(switchtable.back().nextcondi)->SetP(nextcodeOffset());
			}
			if (switchtable.size() > 0) {
				getinsref(switchtable.back().nextin)->SetP(nextcodeOffset());
			}
			//imp 'default'
			auto defaulth = nextToken();
			if (!defaulth.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (defaulth.value().GetType() == TokenType::DEFAULT) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			for (auto& v : switchtable) {
				if (v.type == CDEFAULT) {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			switchtable.emplace_back(CDEFAULT, 0);
			//imp':'
			auto mh = nextToken();
			if (!mh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (mh.value().GetType() == TokenType::MH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< statement >
			auto err = statement();
			if (err.has_value())
				return err;
			switchtable.back().nextin = gencode(JMP, 0);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//'while'||'do'||'for'
	/*<loop_statement> :: =
		'while' '(' < condition > ')' < statement >
		| 'do' < statement > 'while' '(' < condition > ')' ';'
		| 'for' '(' < for_init_statement > [<condition>]';'[<for_update_expression>]')' < statement >*/
	std::optional<CompilationError> Analyser::loop_statement() {
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1 || !p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		if (p1v.GetType() == TokenType::WHILE) {
			int32_t whilestart = nextcodeOffset();
			//imp 'while'
			auto whileh = nextToken();
			if (!whileh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (whileh.value().GetType() == TokenType::WHILE) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< condition > 
			pusht(WHILEC);
			int insjmp;
			auto err = condition(&insjmp);
			if (err.has_value())
				return err;
			popt();
			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			jmpstack.emplace_back(JLOOP);
			//imp< statement >
			err = statement();
			if (err.has_value())
				return err;
			for (auto& v : jmpstack.back().continuel) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			gencode(JMP, whilestart);
			getinsref(insjmp)->SetP(nextcodeOffset());
			for (auto& v : jmpstack.back().breakl) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			jmpstack.pop_back();
		}
		else if (p1v.GetType() == TokenType::DO) {
			int32_t dostart = nextcodeOffset();
			//imp 'do' 
			auto doh = nextToken();
			if (!doh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (doh.value().GetType() == TokenType::DO) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< statement >
			jmpstack.emplace_back(JLOOP);
			auto err = statement();
			if (err.has_value())
				return err;
			for (auto& v : jmpstack.back().continuel) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			//imp 'while'
			auto whileh = nextToken();
			if (!whileh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (whileh.value().GetType() == TokenType::WHILE) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< condition > 
			pusht(DOWHILEC);
			int insjmp;
			err = condition(&insjmp);
			if (err.has_value())
				return err;
			popt();
			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp';'
			auto fh = nextToken();
			if (!fh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (fh.value().GetType() == TokenType::FH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			for (auto& v : jmpstack.back().breakl) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			jmpstack.pop_back();
			getinsref(insjmp)->SetP(dostart);
		}
		else if (p1v.GetType() == TokenType::FOR) {
			int insjmp = -1;
			//imp 'for' 
			auto forh = nextToken();
			if (!forh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (forh.value().GetType() == TokenType::FOR) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< for_init_statement > 
			auto err = for_init_statement();
			if (err.has_value())
				return err;
			//imp[]
			int32_t forstart = nextcodeOffset();
			//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
			auto p1 = peek(1);
			if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::JIA
				|| p1[0].value().GetType() == TokenType::JIAN
				|| p1[0].value().GetType() == TokenType::YKHZ
				|| p1[0].value().GetType() == TokenType::IDENTIFIER
				|| p1[0].value().GetType() == TokenType::UNSIGNED_INTEGER
				|| p1[0].value().GetType() == TokenType::CHARLITER
				|| p1[0].value().GetType() == TokenType::DOUBLELITER)) {
				//imp<condition>
				pusht(FORC);
				auto err = condition(&insjmp);
				if (err.has_value())
					return err;
				popt();
			}

			//imp';'
			auto fh = nextToken();
			if (!fh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (fh.value().GetType() == TokenType::FH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			incodeBuffer();
			//imp[]
			p1 = peek(1);
			if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::IDENTIFIER)) {
				//imp<for_update_expression>
				auto err = for_update_expression();
				if (err.has_value())
					return err;

			}
			auto bf = outcodeBuffer();
			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< statement >
			jmpstack.emplace_back(JLOOP);
			err = statement();
			if (err.has_value())
				return err;
			for (auto& v : jmpstack.back().continuel) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			addbuffer(bf);
			gencode(JMP, forstart);
			if (insjmp != -1) {
				getinsref(insjmp)->SetP(nextcodeOffset());
			}
			for (auto& v : jmpstack.back().breakl) {
				getinsref(v)->SetP(nextcodeOffset());
			}
			jmpstack.pop_back();
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//<identifier>(var)||';'
	/*<for_init_statement> :: =
		[<assignment_expression>{',' < assignment_expression > }]';'*/
	std::optional<CompilationError> Analyser::for_init_statement() {
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::IDENTIFIER)) {
			//imp<assignment_expression>
			auto err = assignment_expression();
			if (err.has_value())
				return err;
			//imp{}
			auto p1 = peek(1);
			while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DOU)) {
				//imp','
				auto dh = nextToken();
				if (!dh.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (dh.value().GetType() == TokenType::DOU) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}

				//imp< assignment_expression >
				auto err = assignment_expression();
				if (err.has_value())
					return err;
				p1 = peek(1);
			}


		}
		//imp';'
		auto fh = nextToken();
		if (!fh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (fh.value().GetType() == TokenType::FH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}

		return {};
	}
	//<identifier>(var)||< identifier>(fun)
	/*<for_update_expression> :: =
		(<assignment_expression> | <function_call>){ ','(<assignment_expression> | <function_call>) }*/
	std::optional<CompilationError> Analyser::for_update_expression() {
		//imp(<assignment_expression> | <function_call>)
		auto p2 = peek(2);
		if (p2.size() != 2 || !p2[0].has_value() || !p2[1].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p2[0].value();
		auto p2v = p2[1].value();
		if (p1v.GetType() == IDENTIFIER && p2v.GetType() == DH) {
			//imp< assignment_expression>
			auto err = assignment_expression();
			if (err.has_value())
				return err;
		}
		else if (p1v.GetType() == IDENTIFIER && p2v.GetType() == YKHZ) {
			//imp< function_call>

			SymType tmp;
			auto err = function_call(&tmp);
			if (err.has_value())
				return err;
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DOU)) {
			//imp','
			auto dh = nextToken();
			if (!dh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (dh.value().GetType() == TokenType::DOU) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp(<assignment_expression> | <function_call>)
			auto p2 = peek(2);
			if (p2.size() != 2 || !p2[0].has_value() || !p2[1].has_value()) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			auto p1v = p2[0].value();
			auto p2v = p2[1].value();
			if (p1v.GetType() == IDENTIFIER&&p2v.GetType() == DH) {
				//imp< assignment_expression>
				auto err = assignment_expression();
				if (err.has_value())
					return err;
			}
			else if (p1v.GetType() == IDENTIFIER&&p2v.GetType() == YKHZ) {
				//imp< function_call>
				SymType tmp;
				auto err = function_call(&tmp);
				if (err.has_value())
					return err;
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			p1 = peek(1);
		}
		return {};
	}
	//'break'||'continue'||'return'
	/*<jump_statement> :: =
		'break' ';'
		| 'continue' ';'
		| <return_statement>*/
	std::optional<CompilationError> Analyser::jump_statement() {
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1 || !p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		if (p1v.GetType() == TokenType::BREAK) {
			//imp 'break' 
			auto breakh = nextToken();
			if (!breakh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (breakh.value().GetType() == TokenType::BREAK) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp';'
			auto fh = nextToken();
			if (!fh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (fh.value().GetType() == TokenType::FH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			if (jmpstack.empty()) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			jmpstack.back().breakl.emplace_back(gencode(JMP,0));
		}
		else if (p1v.GetType() == TokenType::CONTINUE) {
			//imp 'continue'
			auto continueh = nextToken();
			if (!continueh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (continueh.value().GetType() == TokenType::CONTINUE) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp';'
			auto fh = nextToken();
			if (!fh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (fh.value().GetType() == TokenType::FH) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			if (jmpstack.empty()||jmpstack.back().type!=JLOOP) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			jmpstack.back().continuel.emplace_back(gencode(JMP, 0));
		}
		else if (p1v.GetType() == TokenType::RETURN) {
			//imp<return_statement>
			auto err = return_statement();
			if (err.has_value())
				return err;
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//'return'
	/*<return_statement> :: = 'return'[<expression>] ';'*/
	std::optional<CompilationError> Analyser::return_statement() {
		bool hasReturnValue = false;
		SymType dtp = _curfun->returntype;
		//imp 'return'
		auto returnh = nextToken();
		if (!returnh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (returnh.value().GetType() == TokenType::RETURN) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::JIA
			|| p1[0].value().GetType() == TokenType::JIAN
			|| p1[0].value().GetType() == TokenType::YKHZ
			|| p1[0].value().GetType() == TokenType::IDENTIFIER
			|| p1[0].value().GetType() == TokenType::UNSIGNED_INTEGER
			|| p1[0].value().GetType() == TokenType::CHARLITER
			|| p1[0].value().GetType() == TokenType::DOUBLELITER)) {
			//imp<expression>
			SymType etp;
			auto err = expression(&etp);
			if (err.has_value())
				return err;
			hasReturnValue = true;
			if (etp != dtp) {
				if (dtp == INTT) {
					if (etp == DOUBLET) {
						gencode(D2I);
					}
					else if (etp == CHART) {

					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (dtp == DOUBLET) {
					if (etp == INTT) {
						gencode(I2D);
					}
					else if (etp == CHART) {
						gencode(I2D);
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (dtp == CHART) {
					if (etp == DOUBLET) {
						gencode(D2I);
						gencode(I2C);
					}
					else if (etp == INTT) {
						gencode(I2C);
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
		}
		//imp';'
		auto fh = nextToken();
		if (!fh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (fh.value().GetType() == TokenType::FH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		if (dtp != VOIDT && !hasReturnValue) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		if (dtp == CHART) {
			gencode(IRET);
		}
		else if (dtp == DOUBLET) {
			gencode(DRET);
		}
		else if (dtp == INTT) {
			gencode(IRET);
		}
		else if (dtp == VOIDT) {
			gencode(RET);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//'scan'
	/*<scan_statement> :: =
		'scan' '(' < identifier > ')' ';'*/
	std::optional<CompilationError> Analyser::scan_statement() {
		int32_t leveldiff;
		//imp 'scan' 
		auto scanh = nextToken();
		if (!scanh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (scanh.value().GetType() == TokenType::SCAN) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp'('
		auto khz = nextToken();
		if (!khz.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khz.value().GetType() == TokenType::YKHZ) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp < identifier >
		auto id = nextToken();
		if (!id.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (id.value().GetType() == TokenType::IDENTIFIER) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp')'
		auto khy = nextToken();
		if (!khy.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khy.value().GetType() == TokenType::YKHY) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		} 
		//imp';'
		auto fh = nextToken();
		if (!fh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (fh.value().GetType() == TokenType::FH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto vo = val_findinall(id.value().GetValueString());
		if (!vo.has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		if (vo.value().type == CHART) {
			if (vo.value().isGlobal && _curfun != NULL)leveldiff = 1;
			else leveldiff = 0;
			gencode(LOADA, leveldiff, vo.value().offset);
			gencode(CSCAN);
			gencode(ISTORE);
		}
		else if (vo.value().type == INTT) {
			if (vo.value().isGlobal && _curfun != NULL)leveldiff = 1;
			else leveldiff = 0;
			gencode(LOADA, leveldiff, vo.value().offset);
			gencode(ISCAN);
			gencode(ISTORE);
		}
		else if (vo.value().type == DOUBLET) {
			if (vo.value().isGlobal && _curfun != NULL)leveldiff = 1;
			else leveldiff = 0;
			gencode(LOADA, leveldiff, vo.value().offset);
			gencode(DSCAN);
			gencode(DSTORE);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//'print'
	/*<print_statement> :: =
		'print' '('[<printable_list>] ')' ';'*/
	std::optional<CompilationError> Analyser::print_statement() {
		//imp 'print'
		auto printh = nextToken();
		if (!printh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (printh.value().GetType() == TokenType::PRINT) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp'('
		auto khz = nextToken();
		if (!khz.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khz.value().GetType() == TokenType::YKHZ) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		////<string_literal>||<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::JIA
			|| p1[0].value().GetType() == TokenType::JIAN
			|| p1[0].value().GetType() == TokenType::YKHZ
			|| p1[0].value().GetType() == TokenType::IDENTIFIER
			|| p1[0].value().GetType() == TokenType::UNSIGNED_INTEGER
			|| p1[0].value().GetType() == TokenType::CHARLITER
			|| p1[0].value().GetType() == TokenType::DOUBLELITER
			|| p1[0].value().GetType() == TokenType::STRINGLITER)) {
			//imp<printable_list>
			auto err = printable_list();
			if (err.has_value())
				return err;

		}
		//imp')'
		auto khy = nextToken();
		if (!khy.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khy.value().GetType() == TokenType::YKHY) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp';'
		auto fh = nextToken();
		if (!fh.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (fh.value().GetType() == TokenType::FH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		gencode(PRINTL);
		return {};
	}
	//<string_literal>||<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<printable_list>  :: =
		<printable>{ ',' < printable > }*/
	std::optional<CompilationError> Analyser::printable_list() {
		//imp<printable>
		auto err = printable();
		if (err.has_value())
			return err;
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DOU)) {
			//imp','
			auto dh = nextToken();
			if (!dh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (dh.value().GetType() == TokenType::DOU) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			gencode(BIPUSH, 32);
			gencode(CPRINT);
			//imp< printable >
			auto err = printable();
			if (err.has_value())
				return err;
			p1 = peek(1);
		}
		return {};
	}
	//<string_literal>||<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<printable> :: =
		<expression> | <string_literal>*/
	std::optional<CompilationError> Analyser::printable() {
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1 || !p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		////<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
		if (p1v.GetType() == TokenType::JIA
			|| p1v.GetType() == TokenType::JIAN
			|| p1v.GetType() == TokenType::YKHZ
			|| p1v.GetType() == TokenType::IDENTIFIER
			|| p1v.GetType() == TokenType::UNSIGNED_INTEGER
			|| p1v.GetType() == TokenType::CHARLITER
			|| p1v.GetType() == TokenType::DOUBLELITER) {
			//imp<expression>
			SymType etp;
			auto err = expression(&etp);
			if (err.has_value())
				return err;
			if (etp == INTT) {
				gencode(IPRINT);
			}
			else if (etp == DOUBLET) {
				gencode(DPRINT);
			}
			else if (etp == CHART) {
				gencode(CPRINT);
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}

		}
		else if (p1v.GetType() == TokenType::STRINGLITER) {
			//imp<string_literal>
			auto sl = nextToken();
			if (!sl.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (sl.value().GetType() == TokenType::STRINGLITER) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			int32_t sindex = const_getindex(STRINGT, sl.value().GetValue());
			gencode(LOADC, sindex);
			gencode(SPRINT);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//<identifier>
	/*<assignment_expression> :: =
		<identifier><assignment_operator><expression>*/
	std::optional<CompilationError> Analyser::assignment_expression() {
		int32_t leveldiff;
		SymType rtp;
		SymType ltp;
		std::optional<Sym> isym;
		//imp<identifier>
		auto id = nextToken();
		if (!id.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (id.value().GetType() == TokenType::IDENTIFIER) {
			 isym = val_findinall(id.value().GetValueString());
			if (!isym.has_value()) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			if (isym.value().isConst) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			ltp = isym.value().type;
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		if (ltp == CHART) {
			if (isym.value().isGlobal && _curfun != NULL)leveldiff = 1;
			else leveldiff = 0;
			gencode(LOADA, leveldiff, isym.value().offset);
		}
		else if (ltp == DOUBLET) {
			if (isym.value().isGlobal && _curfun != NULL)leveldiff = 1;
			else leveldiff = 0;
			gencode(LOADA, leveldiff, isym.value().offset);
		}
		else if (ltp == INTT) {
			if (isym.value().isGlobal && _curfun != NULL)leveldiff = 1;
			else leveldiff = 0;
			gencode(LOADA, leveldiff, isym.value().offset);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp<assignment_operator>
		auto ao = nextToken();
		if (!ao.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (ao.value().GetType() == TokenType::DH) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		
		//imp<expression>
		auto err = expression(&rtp);
		if (err.has_value())
			return err;
		if (ltp != rtp) {
			if (ltp == INTT) {
				if (rtp == DOUBLET) {
					gencode(D2I);
				}
				else if (rtp == CHART) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (ltp == DOUBLET) {
				if (rtp == INTT) {
					gencode(I2D);
				}
				else if (rtp == CHART) {
					gencode(I2D);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (ltp == CHART) {
				if (rtp == DOUBLET) {
					gencode(D2I);
					gencode(I2C);
				}
				else if (rtp == INTT) {
					gencode(I2C);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		
		if (ltp == CHART) {
			gencode(ISTORE);
		}
		else if (ltp == DOUBLET) {
			gencode(DSTORE);
		}
		else if (ltp == INTT) {
			gencode(ISTORE);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<expression> :: =
		<additive_expression>*/
	std::optional<CompilationError> Analyser::expression(SymType* etp) {
		//imp<additive_expression>
		auto err = additive_expression(etp);
		if (err.has_value())
			return err;
		return {};
	}
	//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<additive_expression> :: =
		<multiplicative_expression>{ <additive_operator><multiplicative_expression> }*/
	std::optional<CompilationError> Analyser::additive_expression(SymType* etp) {
		SymType restp = NOPE;
		SymType rtp = NOPE;
		SymType ltp = NOPE;
		//imp<multiplicative_expression>
		auto err = multiplicative_expression(&ltp);
		if (err.has_value())
			return err;
		
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1&&p1[0].has_value()&&(p1[0].value().GetType()== TokenType::JIA
			|| p1[0].value().GetType() == TokenType::JIAN)) {
			TokenType opt;
			//imp<additive_operator>
			auto ao = nextToken();
			if (!ao.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (ao.value().GetType() == TokenType::JIA) {
				opt = JIA;
			}
			else if (ao.value().GetType() == TokenType::JIAN) {
				opt = JIAN;
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}

			//imp<multiplicative_expression>
			auto tmpf = *_curfun;
			auto tmpp = _offset;
			SymType tmptp;
			auto err = multiplicative_expression(&rtp);
			if (err.has_value())
				return err;
			
			restp = ltp;
			if (ltp != rtp) {
				if (ltp == INTT) {
					if (rtp == DOUBLET) {
						*_curfun = tmpf;
						_offset = tmpp;
						gencode(I2D);
						auto err = multiplicative_expression(&tmptp);
						if (err.has_value())
							return err;
						restp = DOUBLET;
					}
					else if (rtp == CHART) {
						restp = INTT;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ltp == DOUBLET) {
					if (rtp == INTT) {
						gencode(I2D);
						restp = DOUBLET;
					}
					else if (rtp == CHART) {
						gencode(I2D);
						restp = DOUBLET;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ltp == CHART) {
					if (rtp == DOUBLET) {
						*_curfun = tmpf;
						_offset = tmpp;
						gencode(I2D);
						auto err = multiplicative_expression(&tmptp);
						if (err.has_value())
							return err;
						restp = DOUBLET;
					}
					else if (rtp == INTT) {
						restp = INTT;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				restp = ltp;
				if (restp == CHART)restp = INTT;
			}
			ltp = restp;
			p1 = peek(1);
			if (restp == INTT) {
				if (opt == JIA) {
					gencode(IADD);
				}
				else if (opt == JIAN) {
					gencode(ISUB);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (restp == DOUBLET) {
				if (opt == JIA) {
					gencode(DADD);
				}
				else if (opt == JIAN) {
					gencode(DSUB);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		*etp = ltp;

		return {};
	}
	//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<multiplicative_expression> :: =
		<cast_expression>{ <multiplicative_operator><cast_expression> }*/
	std::optional<CompilationError> Analyser::multiplicative_expression(SymType* etp) {
		SymType restp = NOPE;
		SymType rtp = NOPE;
		SymType ltp = NOPE;
		//imp<cast_expression>
		auto err = cast_expression(&ltp);
		if (err.has_value())
			return err;
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::CHENG
			|| p1[0].value().GetType() == TokenType::CHU)) {
			TokenType opt;
			//imp<multiplicative_operator>
			auto mo = nextToken();
			if (!mo.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (mo.value().GetType() == TokenType::CHENG) {
				opt = CHENG;
			}
			else if (mo.value().GetType() == TokenType::CHU) {
				opt = CHU;
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}

			//imp<cast_expression>
			auto tmpf = *_curfun;
			auto tmpp = _offset;
			SymType tmptp;
			auto err = cast_expression(&rtp);
			if (err.has_value())
				return err;
			restp = ltp;
			if (ltp != rtp) {
				if (ltp == INTT) {
					if (rtp == DOUBLET) {
						*_curfun = tmpf;
						_offset = tmpp;
						gencode(I2D);
						auto err = cast_expression(&tmptp);
						if (err.has_value())
							return err;
						restp = DOUBLET;
					}
					else if (rtp == CHART) {
						restp = INTT;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ltp == DOUBLET) {
					if (rtp == INTT) {
						gencode(I2D);
						restp = DOUBLET;
					}
					else if (rtp == CHART) {
						gencode(I2D);
						restp = DOUBLET;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ltp == CHART) {
					if (rtp == DOUBLET) {
						*_curfun = tmpf;
						_offset = tmpp;
						gencode(I2D);
						auto err = cast_expression(&tmptp);
						if (err.has_value())
							return err;
						restp = DOUBLET;
					}
					else if (rtp == INTT) {
						restp = INTT;
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				restp = ltp;
				if (restp == CHART)restp = INTT;
			}
			ltp = restp;
			p1 = peek(1);
			if (restp == INTT) {
				if (opt == CHENG) {
					gencode(IMUL);
				}
				else if (opt == CHU) {
					gencode(IDIV);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (restp == DOUBLET) {
				if (opt == CHENG) {
					gencode(DMUL);
				}
				else if (opt == CHU) {
					gencode(DDIV);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		*etp = ltp;
		return {};
	}
	//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<cast_expression> :: =
	{ '(' < type_specifier > ')' }<unary_expression>*/
	std::optional<CompilationError> Analyser::cast_expression(SymType* etp) {
		std::vector<SymType> tmp;
		SymType stp;
		SymType dtp;
		//imp{}
		auto p2 = peek(2);
		while (p2.size() == 2 && p2[0].has_value() &&
			p2[1].has_value() &&
			(p2[0].value().GetType() == TokenType::YKHZ)&&
			(p2[1].value().GetType() == TokenType::VOID||
				p2[1].value().GetType() == TokenType::INT||
				p2[1].value().GetType() == TokenType::CHAR||
				p2[1].value().GetType() == TokenType::DOUBLE)
			) {
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< type_specifier >
			auto ts = nextToken();
			if (!ts.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (ts.value().GetType() == TokenType::VOID) {
				tmp.emplace_back(t2t(ts.value()));
			}
			else if (ts.value().GetType() == TokenType::INT) {
				tmp.emplace_back(t2t(ts.value()));
			}
			else if (ts.value().GetType() == TokenType::CHAR) {
				tmp.emplace_back(t2t(ts.value()));
			}
			else if (ts.value().GetType() == TokenType::DOUBLE) {
				tmp.emplace_back(t2t(ts.value()));
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}

			p2 = peek(2);
		}
		//imp<unary_expression>
		auto err = unary_expression(&stp);
		if (err.has_value())
			return err;
		for (int i = tmp.size() - 1; i >= 0; --i) {
			dtp = tmp[i];
			if (dtp == INTT) {
				if (stp == DOUBLET) {
					gencode(D2I);
				}
				else if (stp == CHART) {

				}
				else if(stp == INTT){}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (dtp == DOUBLET) {
				if (stp == INTT) {
					gencode(I2D);
				}
				else if (stp == CHART) {
					gencode(I2D);
				}
				else if (stp == DOUBLET) {}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (dtp == CHART) {
				if (stp == DOUBLET) {
					gencode(D2I);
					gencode(I2C);
				}
				else if (stp == INTT) {
					gencode(I2C);
				}
				else if(stp==CHART){
					
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			stp = dtp;
		}
		*etp = stp;
		return {};
	}
	//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<unary_expression> :: =
		[<unary_operator>]<primary_expression>*/
	std::optional<CompilationError> Analyser::unary_expression(SymType* etp) {
		TokenType opt = NULL_TOKEN;
		SymType stp;
		//imp[]
		auto p1 = peek(1);
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::JIA
			|| p1[0].value().GetType() == TokenType::JIAN)) {
			//imp<unary_operator>
			auto uo = nextToken();
			if (!uo.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (uo.value().GetType() == TokenType::JIA) {
				opt = JIA;
			}
			else if (uo.value().GetType() == TokenType::JIAN) {
				opt = JIAN;
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		//imp<primary_expression>
		auto err = primary_expression(&stp);
		if (err.has_value())
			return err;
		//waring 
		if (opt == JIAN) {
			if (stp == INTT) {
				gencode(INEG);
			}
			else if (stp == DOUBLET) {
				gencode(DNEG);
			}
			else if (stp == CHART) {
				gencode(INEG);
			}
			else{
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		else if(opt==JIA){
			if (stp == CHART) {
				stp = INTT;
			}
			if (stp == INTT) {
				
			}
			else if (stp == DOUBLET) {

			}
			else if (stp == CHART) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		*etp = stp;
		return {};
	}
	//'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
	/*<primary_expression> :: =
		'(' < expression > ')'
		| <identifier>
		| <integer_literal>
		| <char_literal>
		| <floating_literal>
		| <function_call>*/
	std::optional<CompilationError> Analyser::primary_expression(SymType* etp) {
		int32_t leveldiff;
		int32_t cindex;
		//imp|
		auto p1 = peek(1);
		if (p1.size() != 1||!p1[0].has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto p1v = p1[0].value();
		if (p1v.GetType() == TokenType::IDENTIFIER) {
			//imp(<identifier> | <function_call>)
			auto p2 = peek(2);
			if (p2.size() != 2 || !p2[0].has_value() || !p2[1].has_value()) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			auto p2v = p2[1].value();
			if (p2v.GetType() == YKHZ) {
				//imp<function_call>
				auto err = function_call(etp);
				if (err.has_value())
					return err;
				//enhanced!
				if (*etp == VOIDT)return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			else {
				//imp<identifier>
				auto id = nextToken();
				if (!id.has_value())
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				if (id.value().GetType() == TokenType::IDENTIFIER) {
					auto vo = val_findinall(id.value().GetValueString());
					if (!vo.has_value()) {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
					*etp = vo.value().type;
					if (vo.value().isGlobal && _curfun != NULL)leveldiff = 1;
					else leveldiff = 0;
					gencode(LOADA, leveldiff, vo.value().offset);
					if (*etp == INTT) {
						gencode(ILOAD);
					}
					else if (*etp == CHART) {
						gencode(ILOAD);
					}
					else if (*etp == DOUBLET) {
						gencode(DLOAD);
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
		}
		else if (p1v.GetType() == TokenType::YKHZ) {
			//imp'('
			auto khz = nextToken();
			if (!khz.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khz.value().GetType() == TokenType::YKHZ) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp< expression >
			auto err = expression(etp);
			if (err.has_value())
				return err;
			//imp')'
			auto khy = nextToken();
			if (!khy.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (khy.value().GetType() == TokenType::YKHY) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		else if (p1v.GetType() == TokenType::UNSIGNED_INTEGER) {
			//imp<integer_literal>
			auto il = nextToken();
			if (!il.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (il.value().GetType() == TokenType::UNSIGNED_INTEGER) {
				cindex = const_getindex(INTT, il.value().GetValue());
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			*etp = INTT;
			gencode(LOADC, cindex);
		}
		else if (p1v.GetType() == TokenType::CHARLITER) {
			//imp<char_literal>
			auto cl = nextToken();
			if (!cl.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (cl.value().GetType() == TokenType::CHARLITER) {
				cindex = const_getindex(CHART, cl.value().GetValue());
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			*etp = CHART;
			gencode(LOADC, cindex);
		}
		else if (p1v.GetType() == TokenType::DOUBLELITER) {
			//imp<floating_literal>
			auto fl = nextToken();
			if (!fl.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (fl.value().GetType() == TokenType::DOUBLELITER) {
				cindex = const_getindex(DOUBLET, fl.value().GetValue());
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			*etp = DOUBLET;
			gencode(LOADC, cindex);
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}

		return {};
	}
	//OK
	//< identifier>(fun)
	/*<function_call> :: =
		< identifier> '('[<expression_list>] ')'*/
	std::optional<CompilationError> Analyser::function_call(SymType* etp) {
		int32_t findex;
		//imp<identifier>
		auto id = nextToken();
		if (!id.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (id.value().GetType() == TokenType::IDENTIFIER) {
			auto vo = val_findinall(id.value().GetValueString());
			if (!vo.has_value()||vo.value().type!=FUNT) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			auto fo = findfun(id.value().GetValueString());
			if (!fo.has_value()) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			findex = fo.value().index;
		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		pusht(id.value());
		//imp'('
		auto khz = nextToken();
		if (!khz.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khz.value().GetType() == TokenType::YKHZ) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		//imp[]
		auto p1 = peek(1);
		//<unary_operator>||'('||< identifier >(var)||<integer_literal>||<char_literal>||<floating_literal>||< identifier >(fun)
		if (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == JIA
			|| p1[0].value().GetType() == JIAN
			|| p1[0].value().GetType() == YKHZ
			|| p1[0].value().GetType() == IDENTIFIER
			|| p1[0].value().GetType() == UNSIGNED_INTEGER
			|| p1[0].value().GetType() == CHARLITER
			|| p1[0].value().GetType() == DOUBLELITER)) {
			//imp<expression_list>
			auto err = expression_list();
			if (err.has_value())
				return err;

		}
		//imp')'
		auto khy = nextToken();
		if (!khy.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		if (khy.value().GetType() == TokenType::YKHY) {

		}
		else {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto fo = findfun(id.value().GetValueString());
		if (!fo.has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		else {
			*etp = fo.value().returntype;
		}
		popt();
		//code
		gencode(CALL, findex);
		return {};
	}
	/*<expression_list> :: =
		<expression>{ ',' < expression > }*/
	std::optional<CompilationError> Analyser::expression_list() {
		int pcnt = 0;
		//imp<expression>
		SymType pt;
		SymType ptt;
		auto err = expression(&pt);
		if (err.has_value())
			return err;
		auto fo = findfun(topt().value().GetValueString());
		if (!fo.has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		if (pcnt > fo.value().params.size() - 1) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		auto po = fo.value().params[pcnt++];
		ptt = po.type;
		if (ptt != pt) {
			if (ptt == INTT) {
				if (pt == DOUBLET) {
					gencode(D2I);
				}
				else if (pt == CHART) {

				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (ptt == DOUBLET) {
				if (pt == INTT) {
					gencode(I2D);
				}
				else if (pt == CHART) {
					gencode(I2D);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else if (ptt == CHART) {
				if (pt == DOUBLET) {
					gencode(D2I);
					gencode(I2C);
				}
				else if (pt == INTT) {
					gencode(I2C);
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
		}
		//imp{}
		auto p1 = peek(1);
		while (p1.size() == 1 && p1[0].has_value() && (p1[0].value().GetType() == TokenType::DOU)) {
			//imp','
			auto dh = nextToken();
			if (!dh.has_value())
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			if (dh.value().GetType() == TokenType::DOU) {

			}
			else {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			//imp<expression>
			auto err = expression(&pt);
			if (err.has_value())
				return err;
			if (pcnt > fo.value().params.size() - 1) {
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
			}
			auto po = fo.value().params[pcnt++];
			ptt = po.type;
			if (ptt != pt) {
				if (ptt == INTT) {
					if (pt == DOUBLET) {
						gencode(D2I);
					}
					else if (pt == CHART) {

					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ptt == DOUBLET) {
					if (pt == INTT) {
						gencode(I2D);
					}
					else if (pt == CHART) {
						gencode(I2D);
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else if (ptt == CHART) {
					if (pt == DOUBLET) {
						gencode(D2I);
						gencode(I2C);
					}
					else if (pt == INTT) {
						gencode(I2C);
					}
					else {
						return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
					}
				}
				else {
					return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
				}
			}
			p1 = peek(1);
		}
		return {};
	}

	std::vector<std::optional<Token> > Analyser::peek(int t) {
		std::vector<std::optional<Token> > res;
		for (int i = 0; i < t && _offset + i < _tokens.size(); ++i) {
			res.emplace_back(_tokens[_offset + i]);
		}
		return res;
	}


	std::optional<Token> Analyser::nextToken() {
		if (_offset == _tokens.size())
			return {};
		// 考虑到 _tokens[0..._offset-1] 已经被分析过了
		// 所以我们选择 _tokens[0..._offset-1] 的 EndPos 作为当前位置
		_current_pos = _tokens[_offset].GetEndPos();
		return _tokens[_offset++];
	}


	void Analyser::unreadToken() {
		if (_offset == 0)
			DieAndPrint("analyser unreads token from the begining.");
		_current_pos = _tokens[_offset - 1].GetEndPos();
		_offset--;
	}

	int32_t Analyser::getTypesize(SymType type) {
		switch (type) {
		case CHART:
			return 1;
		case INTT:
			return 1;
		case DOUBLET:
			return 2;
		case FUNT:
			return 0;
		default:
			DieAndPrint("shouldn't be here4.");
		}
		return 0;
	}
	uint16_t Analyser::const_getindex_(SymType type, std::any value) {
		int n = _this.constants.size();
		for (uint16_t i = 0; i < n; ++i) {
			if (_this.constants[i].type == type) {
				Double_info double1, double2;
				Int_info int1, int2;
				String_info string1, string2;
				switch (type) {
				case SymType::DOUBLET:
					double1 = std::any_cast<Double_info>(value);
					double2 = std::any_cast<Double_info>(_this.constants[i].value);
					if (double1 == double2) {
						return i;
					}
					break;
				case SymType::CHART:
				case SymType::INTT:
					int1 = std::any_cast<Int_info>(value);
					int2 = std::any_cast<Int_info>(_this.constants[i].value);
					if (int1 == int2) {
						return i;
					}
					break;
				case SymType::STRINGT:
					string1 = std::any_cast<String_info>(value);
					string2 = std::any_cast<String_info>(_this.constants[i].value);
					if (string1 == string2) {
						return i;
					}
					break;
				default:
					DieAndPrint("shouldn't be here1.");
				}
			}
		}
		_this.constants.emplace_back(type, value);
		return (uint16_t)_this.constants.size() - 1;
	}
	uint16_t Analyser::const_getindex(SymType type, std::any value) {
		switch (type) {
		case SymType::DOUBLET:
			return const_getindex_(type, Double_info(std::any_cast<double>(value)));
			break;
		case SymType::CHART:
			return const_getindex_(INTT, Int_info(std::any_cast<uint8_t>(value)));
			break;
		case SymType::INTT:
			return const_getindex_(type, Int_info(std::any_cast<int32_t>(value)));
			break;
		case SymType::STRINGT:
			return const_getindex_(type, String_info(std::any_cast<std::string>(value)));
			break;
		default:
			DieAndPrint("shouldn't be here2.");
		}
		return -1;
	}
	std::optional<Sym> Analyser::globalVal_find(std::string name) {
		for (auto& v : _globalsyms) {
			if (v.name == name) {
				return std::make_optional<Sym>(v);
			}
		}
		return {};
	}
	std::optional<Sym> Analyser::globalVal_add(bool isConst,SymType type,std::string name) {
		for (auto& v : _globalsyms) {
			if (v.name == name) {
				return {};
			}
		}
		int32_t offset;
		if (_globalsyms.empty()) {
			offset = 0;
		}
		else {
			offset = _globalsyms.back().offset + getTypesize(_globalsyms.back().type);
		}
		//warning
		return std::make_optional<Sym>(_globalsyms.emplace_back(isConst,type, name, offset,true));

	}
	std::optional<Sym> Analyser::val_findinsym(std::vector<Sym>& t, std::string name) {
		for (auto& v : t) {
			if (v.name == name) {
				return std::make_optional<Sym>(v);
			}
		}
		return {};

	}
	std::optional<Sym> Analyser::val_findinall(std::string name) {
		int32_t n = _syms.size();
		for (int32_t i = n - 1; i >= 0;--i) {
			auto res=val_findinsym(_syms[i], name);
			if (res.has_value()) {
				return res;
			}
		}
		auto res = globalVal_find(name);
		if (res.has_value()) {
			return res;
		}
		return{};

	}


	std::optional<CompilationError> Analyser::infunhead(SymType type,std::string name) {
		_syms.emplace_back();//new param and local
		_curfun = new Function_info();
		_curfun->name_index= const_getindex(SymType::STRINGT, name);
		_curfun->name = name;
		_curfun->returntype = type;

		auto res=globalVal_add(true,FUNT, name);
		if (!res.has_value()) {
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::Invalid);
		}
		return {};
	}
	
	void Analyser::infun() {
		//copy params
		_curfun->params = _syms[0];
		uint16_t ps = 0;
		for (auto& v : _curfun->params) {
			ps += getTypesize(v.type);
		}
		_curfun->params_size = ps;
		_curfun->level = 1;
		_this.functions.emplace_back(*_curfun);
		_curfun->index = _this.functions_count;
		_this.functions_count++;
	}
	void Analyser::outfun() {
		_this.functions.pop_back();
		_this.functions.emplace_back(*_curfun);
		_syms.clear();
		
		
		
	}
	void Analyser::val_inblock() {
		_syms.emplace_back();

	}
	void Analyser::val_outblock() {
			for (int i = _syms.back().size() - 1; i >= 0;--i ) {
			switch (_syms.back()[i].type) {
			case INTT:
				gencode(POP);
				break;
			case CHART:
				gencode(POP);
				break;
			case DOUBLET:
				gencode(POP2);
				break;
			default:
				break;
			}
		}
		_syms.pop_back();

	}
	std::optional<Sym> Analyser::val_addinsym(std::vector<Sym>& t, bool isConst, SymType type, std::string name, int32_t offsetas) {
		for (auto& v : t) {
			if (v.name == name) {
				return {};
			}
		}
		int32_t offset;
		if (t.empty()) {
			offset = 0;
		}
		else {
			offset = t.back().offset + getTypesize(t.back().type);
		}
		if (offsetas != -1)offset = offsetas;
		//warning
		return std::make_optional<Sym>(t.emplace_back(isConst,type, name, offset,false));

	}
	std::optional<Sym> Analyser::val_add(bool isConst, SymType type, std::string name) {
		int32_t off = -1;
		if (_syms.empty()) {
			return globalVal_add(isConst, type, name);
		}
		else {
			if (_syms.size() > 1 && _syms.back().empty()) {
				for (int i = _syms.size() - 2; i >= 0; --i) {
					if (!_syms[i].empty()) {
						off = _syms[i].back().offset + getTypesize(_syms[i].back().type);
						break;
					}
				}
			}
			return val_addinsym(_syms.back(), isConst, type, name,off);
		}
		

	}
	

	void Analyser::pusht(Token t) {
		_ttmp.emplace_back(t);
	}
	void Analyser::pusht(TokenType t) {
		_ttmp.emplace_back(t);
	}
	bool Analyser::popt() {
		if (_ttmp.size() == 0)return false;
		else{
			_ttmp.pop_back();
			return true;
		}
	}
	std::optional<Token> Analyser::topt() {
		if (_ttmp.size() == 0)return {};
		else {
			return std::make_optional<Token>(_ttmp.back());
		}
	}
	std::optional<Function_info> Analyser::findfun(std::string name) {
		for (auto& v : _this.functions) {
			if (v.name == name) {
				return std::make_optional<Function_info>(v);
			}
		}
		return {};

	}
	int Analyser::gencode(Operation opr, int32_t p1, int32_t p2) {
		uint16_t offset;
		if (_curfun==NULL) {
			if (_this.start_code.instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _this.start_code.instructions.back().GetOffset() + _this.start_code.instructions.back().Getsize();
			}
			_this.start_code.instructions.emplace_back(opr, p1, p2,offset);
			return _this.start_code.instructions.size() - 1;
		}
		else {
			if (_curfun->instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _curfun->instructions.back().GetOffset() + _curfun->instructions.back().Getsize();
			}
			_curfun->instructions.emplace_back(opr, p1, p2,offset);
			return _curfun->instructions.size() - 1;
		}
	}
	int Analyser::gencode(Operation opr, int32_t p1) {
		uint16_t offset;
		if (_curfun == NULL) {
			if (_this.start_code.instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _this.start_code.instructions.back().GetOffset() + _this.start_code.instructions.back().Getsize();
			}
			_this.start_code.instructions.emplace_back(opr, p1, offset);
			return _this.start_code.instructions.size() - 1;
		}
		else {
			if (_curfun->instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _curfun->instructions.back().GetOffset() + _curfun->instructions.back().Getsize();
			}
			_curfun->instructions.emplace_back(opr, p1, offset);
			return _curfun->instructions.size() - 1;
		}
	}
	int Analyser::gencode(Operation opr) {
		uint16_t offset;
		if (_curfun == NULL) {
			if (_this.start_code.instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _this.start_code.instructions.back().GetOffset() + _this.start_code.instructions.back().Getsize();
			}
			_this.start_code.instructions.emplace_back(opr,offset);
			return _this.start_code.instructions.size() - 1;
		}
		else {
			if (_curfun->instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _curfun->instructions.back().GetOffset() + _curfun->instructions.back().Getsize();
			}
			_curfun->instructions.emplace_back(opr, offset);
			return _curfun->instructions.size() - 1;
		}
	}
	int Analyser::gencode() {
		uint16_t offset;
		if (_curfun == NULL) {
			if (_this.start_code.instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _this.start_code.instructions.back().GetOffset() + _this.start_code.instructions.back().Getsize();
			}
			_this.start_code.instructions.emplace_back(offset);
			return _this.start_code.instructions.size() - 1;
		}
		else {
			if (_curfun->instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _curfun->instructions.back().GetOffset() + _curfun->instructions.back().Getsize();
			}
			_curfun->instructions.emplace_back(offset);
			return _curfun->instructions.size() - 1;
		}
	}
	SymType Analyser::t2t(Token t) {
		switch (t.GetType()) {
		case CHAR:
			return CHART;
		case INT:
			return INTT;
		case DOUBLE:
			return DOUBLET;
		case VOID:
			return VOIDT;
		default:
			//todo:error
			return INTT;
		}
		
	}
	void Analyser::incodeBuffer() {
		_bufferstart = _curfun->instructions.size();

	}
	std::vector<Instruction>* Analyser::outcodeBuffer() {
		std::vector<Instruction>* res = new std::vector<Instruction>(_curfun->instructions.begin()+_bufferstart, _curfun->instructions.end());
		_curfun->instructions.erase(_curfun->instructions.begin() + _bufferstart, _curfun->instructions.end());
		return res;
	}
	void Analyser::addbuffer(std::vector<Instruction>* buffer) {
		for (auto& v:*buffer) {
			gencode(v.GetOperation(), v.GetP1(), v.GetP2());
		}
		delete buffer;
	}
	uint16_t Analyser::nextcodeOffset() {
		uint16_t offset;
		if (_curfun == NULL) {
			if (_this.start_code.instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _this.start_code.instructions.back().GetOffset() + _this.start_code.instructions.back().Getsize();
			}
		}
		else {
			if (_curfun->instructions.empty()) {
				offset = 0;
			}
			else {
				offset = _curfun->instructions.back().GetOffset() + _curfun->instructions.back().Getsize();
			}
		}
		return offset;
	}
	Instruction* Analyser::getinsref(int i) {
		uint16_t offset;
		if (_curfun == NULL) {
			return &_this.start_code.instructions[i];
		}
		else {
			
			return &_curfun->instructions[i];
		}
	}
	void C0_file::genbinary(std::ostream& out) {
		void* _point;
		char bytes[8];
		char* p;
		// magic
		out.write("\x43\x30\x3A\x29", 4);
		// version
		out.write("\x00\x00\x00\x01", 4);
		// constants_count
		c0::u2 constants_count = constants.size();
		_point = &constants_count;
		p = reinterpret_cast<char*>(_point) + (sizeof constants_count - 1);
		for (int i = 0; i < sizeof constants_count; ++i) {
			bytes[i] = *p--;
		}
		out.write(bytes, sizeof constants_count);
		// constants
		for (auto& constant : constants) {
			switch (constant.type)
			{
			case c0::STRINGT: {
				out.write("\x00", 1);
				std::string v = std::any_cast<String_info>(constant.value).value;
				c0::u2 len = v.length();
				_point = &len;
				char* p = reinterpret_cast<char*>(_point) + (sizeof len - 1);
				for (int i = 0; i < sizeof len; ++i) {
					bytes[i] = *p--;
				}
				out.write(bytes, sizeof len);
				out.write(v.c_str(), len);
			} break;
			case c0::INTT: {
				out.write("\x01", 1);
				std::int32_t v = std::any_cast<Int_info>(constant.value).value;
				_point = &v;
				char* p = reinterpret_cast<char*>(_point) + (sizeof v - 1);
				for (int i = 0; i < sizeof v; ++i) {
					bytes[i] = *p--;
				}
				out.write(bytes, sizeof v);
			} break;
			case c0::DOUBLET: {
				out.write("\x02", 1);
				double v = std::any_cast<Double_info>(constant.value).value;
				_point = &v;
				char* p = reinterpret_cast<char*>(_point) + (sizeof v - 1);
				for (int i = 0; i < sizeof v; ++i) {
					bytes[i] = *p--;
				}
				out.write(bytes, sizeof v);
			} break;
			default: break;
			}
		}
		auto inss = start_code.instructions;
		
		c0::u2 instructions_count = inss.size();
		_point = &instructions_count;
		p = reinterpret_cast<char*>(_point) + (sizeof instructions_count - 1);
		for (int i = 0; i < sizeof instructions_count; ++i) {
			bytes[i] = *p--;
		}
		out.write(bytes, sizeof instructions_count);
		for (auto& ins : inss) {
			c0::u1 op = static_cast<c0::u1>(ins._opr); 
			_point = &op;
			char* p = reinterpret_cast<char*>(_point) + (sizeof op - 1);
			for (int i = 0; i < sizeof op; ++i) {
				bytes[i] = *p--;
			}
			out.write(bytes, sizeof op);
			if (auto it = c0::paramSizeOfOpCode.find(ins._opr); it != c0::paramSizeOfOpCode.end()) {
				auto paramSizes = it->second;
				switch (paramSizes[0]) {
				case 1: {
					c0::u1 p1 = ins._p1;
					_point = &p1;
					char* p = reinterpret_cast<char*>(_point);
					for (int i = 0; i < 1; ++i) {
						bytes[i] = *p--;
					}
					out.write(bytes, 1);
					break;
				}
				case 2: {
					c0::u2 p1 = ins._p1;
					_point = &p1;
					char* p = reinterpret_cast<char*>(_point) + 1;
					for (int i = 0; i < 2; ++i) {
						bytes[i] = *p--;
					}
					out.write(bytes, 2);
					break;
				}
				case 4: {
					c0::u4 p1 = ins._p1;
					_point = &p1;
					char* p = reinterpret_cast<char*>(_point) + 3;
					for (int i = 0; i < 4; ++i) {
						bytes[i] = *p--;
					}
					out.write(bytes, 4);
					break;
				}
				default:break;
				}
				if (paramSizes.size() == 2) {
					switch (paramSizes[1]) {
					case 1: {
						c0::u1 p2 = ins._p2;
						_point = &p2;
						char* p = reinterpret_cast<char*>(_point);
						for (int i = 0; i < 1; ++i) {
							bytes[i] = *p--;
						}
						out.write(bytes, 1);
						break;
					}
					case 2: {
						c0::u2 p2 = ins._p2;
						_point = &p2;
						char* p = reinterpret_cast<char*>(_point) + 1;
						for (int i = 0; i < 2; ++i) {
							bytes[i] = *p--;
						}
						out.write(bytes, 2);
						break;
					}
					case 4: {
						c0::u4 p2 = ins._p2;
						_point = &p2;
						char* p = reinterpret_cast<char*>(_point) + 3;
						for (int i = 0; i < 4; ++i) {
							bytes[i] = *p--;
						}
						out.write(bytes, 4);
						break;
					}
					default:break;
					}
				}
			}
		}
		// functions_count
		c0::u2 functions_count = functions.size();
		_point = &functions_count;
		p = reinterpret_cast<char*>(_point) + (sizeof functions_count - 1);
		for (int i = 0; i < sizeof functions_count; ++i) {
			bytes[i] = *p--;
		}
		out.write(bytes, sizeof functions_count);
		// functions
		for (auto& fun : functions) {
			c0::u2 v;
			_point = &v;
			v = fun.name_index; 
			char* p = reinterpret_cast<char*>(_point) + (sizeof v - 1);
			for (int i = 0; i < sizeof v; ++i) {
				bytes[i] = *p--;
			}
			out.write(bytes, sizeof v);
			v = fun.params_size; 
			p = reinterpret_cast<char*>(_point) + (sizeof v - 1);
			for (int i = 0; i < sizeof v; ++i) {
				bytes[i] = *p--;
			}
			out.write(bytes, sizeof v);
			v = fun.level;     
			p = reinterpret_cast<char*>(_point) + (sizeof v - 1);
			for (int i = 0; i < sizeof v; ++i) {
				bytes[i] = *p--;
			}
			out.write(bytes, sizeof v);
			auto& inssfun = fun.instructions;
			c0::u2 instructions_count = inssfun.size();
			_point = &instructions_count;
			p = reinterpret_cast<char*>(_point) + (sizeof instructions_count - 1);
			for (int i = 0; i < sizeof instructions_count; ++i) {
				bytes[i] = *p--;
			}
			out.write(bytes, sizeof instructions_count);
			for (auto& ins : inssfun) {
				c0::u1 op = static_cast<c0::u1>(ins._opr); 
				_point = &op;
				char* p = reinterpret_cast<char*>(_point) + (sizeof op - 1);
				for (int i = 0; i < sizeof op; ++i) {
					bytes[i] = *p--;
				}
				out.write(bytes, sizeof op);
				if (auto it = c0::paramSizeOfOpCode.find(ins._opr); it != c0::paramSizeOfOpCode.end()) {
					auto paramSizes = it->second;
					switch (paramSizes[0]) {
					case 1: {
						c0::u1 p1 = ins._p1;
						_point = &p1;
						char* p = reinterpret_cast<char*>(_point);
						for (int i = 0; i < 1; ++i) {
							bytes[i] = *p--;
						}
						out.write(bytes, 1);
						break;
					}
					case 2: {
						c0::u2 p1 = ins._p1;
						_point = &p1;
						char* p = reinterpret_cast<char*>(_point) + 1;
						for (int i = 0; i < 2; ++i) {
							bytes[i] = *p--;
						}
						out.write(bytes, 2);
						break;
					}
					case 4: {
						c0::u4 p1 = ins._p1;
						_point = &p1;
						char* p = reinterpret_cast<char*>(_point) + 3;
						for (int i = 0; i < 4; ++i) {
							bytes[i] = *p--;
						}
						out.write(bytes, 4);
						break;
					}
					default:;
					}
					if (paramSizes.size() == 2) {
						switch (paramSizes[1]) {
						case 1: {
							c0::u1 p2 = ins._p2;
							_point = &p2;
							char* p = reinterpret_cast<char*>(_point);
							for (int i = 0; i < 1; ++i) {
								bytes[i] = *p--;
							}
							out.write(bytes, 1);
							break;
						}
						case 2: {
							c0::u2 p2 = ins._p2;
							_point = &p2;
							char* p = reinterpret_cast<char*>(_point) + 1;
							for (int i = 0; i < 2; ++i) {
								bytes[i] = *p--;
							}
							out.write(bytes, 2);
							break;
						}
						case 4: {
							c0::u4 p2 = ins._p2;
							_point = &p2;
							char* p = reinterpret_cast<char*>(_point) + 3;
							for (int i = 0; i < 4; ++i) {
								bytes[i] = *p--;
							}
							out.write(bytes, 4);
							break;
						}
						default: break;
						}
					}
				}
			}
		}
	}
}