#include "task_01.h"


/**
 * Given grammar:
 * <Soft> ::= program <id> ; <block> .
 * <block> ::= <var part> <state part>
 * <var part> ::= var <var dec> : <type> ;
 * <var dec> ::= <id> { , <var dec> }
 * <state part> ::= <compound>
 * <compound> ::= begin <state> { ; <state> } end
 * <state> ::= <assign> | <compound> | <your_other_operations>
 * <assign> ::= <id> := <exp> ;
 * <exp> ::= <id> | <constant>
 * <type> ::= integer
 * <id> ::= a-z
 * <constant> ::= 0-9
 */
enum tokens {
	unknown_tk = -1,//  получаем неизвестный токен
	program_tk = 0, //  program
	var_tk, //  var
	begin_tk, //  begin
	end_tk, //  end
	type_tk, //  type
	id_tk = 8, //  any[aA - zZ][0 - 9]
	constant_tk = 9,    // 0-9
	dot_tk,             // '.'
	comma_tk,           // ','
	ddt_tk,             // ':'
	semi_tk,            // ';'
	eqv_tk,             // '='
	ass_tk,             // ':='
	add_tk,				// '+'
	sub_tk,				// '-'
	mul_tk,				// '*'
	div_tk,				// 'div'
	plus_tk,//'+'
	minus_tk//'-'
};

typedef std::pair<std::string, tokens> lexem;


typedef struct synt {
	char  GetCurrentCurs();     // получили текущий символ в потоке
	lexem GetLex();             // переход к след. лексеме

	char  PeekChar(int n);      // peek on N symbols in stream
	lexem PeekLex(int n);       // peek N lexem;
	bool  FileEOF();
	void  FileOpen(std::string file);
	bool  FileIsOpen();
	void  FileClose();
	bool  FileFail();
	int   GetCurrentLine();
private:
	int LineN = 1;

	std::ifstream code;         // input stream of Pascal code

	char Curs{ -1 };         // cursor of stream

	char getChar();            // get next character in stream
} synt_T;

struct TListElem//список
{
	std::string Info;
	std::string Type;
	TListElem* Link;
};
TListElem* Addinfo(std::string info, std::string Type, TListElem* pList)//добавление в список
{
	TListElem* pElem = new TListElem;
	pElem->Info = info;
	pElem->Type = Type;
	pElem->Link = pList;

	return pElem;
}
int SrList(TListElem* pList, std::string ident)//поиск в списке
{
	TListElem* pElem = pList;
	while (pElem != NULL)
	{
		if (pElem->Info == ident)
			return 1;
		pElem = pElem->Link;
	}
	return 0;
}
//структура для работы с переменными
struct variable {
	void VarSave(std::string ident, std::string Type);//сохранение переменной
	int VarSearch(std::string ident);//поиск переменной
	TListElem* VarAr = NULL;
} VarList;

void variable::VarSave(std::string ident, std::string Type)
{
	VarAr = Addinfo(ident, Type, VarAr);
}
int variable::VarSearch(std::string ident)
{
	return SrList(VarAr, ident);
}
void ErrMsg(int StrN, lexem Lex, std::string Type)//вывод инф. об ошибках
{

	//if (StrN != "")
		std::cout << "Ошибка в строке " << StrN << " in lex: \"" << Lex.first << "\"\nТип ошибки: " << Type << std::endl;
	//else
		std::cout << "Ошибка в строке " << StrN << std::endl;
}
void ErrMsg(std::string Type)
{
	std::cout << "\nТип ошибки: " << Type << std::endl;
}
void ErrMsg(int StrN, char ch, std::string Type)
{
	std::cout << "Ошибка в строке " << StrN << " in lex: \"" << ch << "\"\nТип ошибки: " << Type << std::endl;
}
char synt_T::getChar() {
	if (code.fail()) {
		ErrMsg("Неудается прочитать из файла");
		std::cerr << "<G> Неудается прочитать из файла" << std::endl;
		throw std::runtime_error("Файл недоступен");
	}

	if (!code.eof()) {
		code >> std::noskipws >> Curs;
	}
	else {
		ErrMsg("File is EOF early");
		std::cerr << "<G> File is EOF early" << std::endl;
		throw std::runtime_error("File is EOF early");
	}

	return Curs;
}
char synt_T::PeekChar(int n) {
	try {
		char ch = -1;
		int curr_pos = code.tellg(); // получить текущую позицию в потоке

		code.seekg(curr_pos + n, code.beg); // установите необходимое положение в потоке
		code >> std::noskipws >> ch;    // получить символ из потока с помощью ' '
		code.seekg(curr_pos, code.beg); // возврат предыдущей позиции в потоке


		return ch;
	}
	catch (std::exception& exp) {
		std::cerr << "<G> Исключение в " << __func__ << ": " << exp.what()
			<< std::endl;
		return -1;
	}
}
char synt_T::GetCurrentCurs() {
	return Curs;
}
lexem synt_T::GetLex() {
	try {
		auto ch = GetCurrentCurs();
		while (ch == -1 || ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t') {
			if (ch == '\n')
				LineN++;
			ch = getChar();
		}

		auto isId = [](char ch) {
			return std::isalpha(static_cast<unsigned char>(ch)) ||
				std::isdigit(static_cast<unsigned char>(ch));
		};


		std::string Lex;
		if (std::isdigit(static_cast<unsigned char>(ch))) { // Constants (Numbers)
			while (std::isdigit(static_cast<unsigned char>(ch))) {
				Lex += ch;
				ch = getChar();
			}

			return std::make_pair(Lex, constant_tk);
		}
		else if (std::isalpha(static_cast<unsigned char>(ch))) { // Identificators
			while (isId(ch)) {
				Lex += ch;
				ch = getChar();
			}

			if (Lex == "program") { return std::make_pair(Lex, program_tk); }
			else if (Lex == "var") { return std::make_pair(Lex, var_tk); }
			else if (Lex == "begin") { return std::make_pair(Lex, begin_tk); }
			else if (Lex == "integer") { return std::make_pair(Lex, type_tk); }
			else if (Lex == "end") { return std::make_pair(Lex, end_tk); }
			else { // it is ID
				return std::make_pair(Lex, id_tk);
			}
		}
		else if (std::ispunct(static_cast<unsigned char>(ch))) { // Прочие символы
			tokens tok;
			switch (ch) {
			case ',': tok = comma_tk; break;
			case '.': tok = dot_tk;   break;
			case ':': tok = ddt_tk;   break;
			case ';': tok = semi_tk;  break;
			case '=': tok = eqv_tk;   break;
			case '+': tok = add_tk;   break;
			case '-': tok = sub_tk;   break;
			case '*': tok = mul_tk;   break;
			case '/': tok = div_tk;   break;
			default: {
				ErrMsg(GetCurrentLine(), ch, "Неизвестный токен");
				std::cerr << "<G> Неизвестный токен " << ch << std::endl; break;
			}
			}
			Lex += ch;

			if (tok == ddt_tk) {
				ch = getChar();
				if (ch == '=') {
					Lex += ch;
					tok = ass_tk;
				}
			}

			getChar();
			return std::make_pair(Lex, tok);
		}
		else {
			ErrMsg(GetCurrentLine(), ch, "Неизвестный токен");
			std::cerr << "<G> Неизвестный токен " << ch << std::endl;
		}

		return std::make_pair("", unknown_tk);
	}
	catch (const std::exception& exp) {
		return std::make_pair("", unknown_tk);
	}
}
lexem synt_T::PeekLex(int n) {
	int curr_pos = code.tellg(); // получить текущую позицию в потоке
	auto Curr_Curs = GetCurrentCurs();
	try {
		lexem res;
		for (int i = 0; i < n; i++) {
			res = GetLex();
		}
		code.seekg(curr_pos, code.beg);
		Curs = Curr_Curs;

		return res;
	}
	catch (const std::exception& exp) {
		ErrMsg("Вы пытаетесь заглянуть слишком далеко вперед, вернитесь");
		std::cerr << "<G> Вы пытаетесь заглянуть слишком далеко вперед, вернитесь" << std::endl;
		code.seekg(curr_pos, code.beg);
		Curs = Curr_Curs;

		return std::make_pair("", unknown_tk);
	}
}
bool synt_T::FileEOF()
{
	return code.eof();
}
void  synt_T::FileOpen(std::string file)
{
	code.open(file);
}
bool  synt_T::FileIsOpen()
{
	return code.is_open();
}
void  synt_T::FileClose()
{
	code.close();
}
bool  synt_T::FileFail()
{
	return code.fail();
}
int   synt_T::GetCurrentLine()
{
	return LineN;
}
void  buildTreeStub(lexem lex);
int   expressionParse(lexem lex, synt_T& parser);
int   stateParse(lexem& lex, synt_T& parser);
int   compoundParse(lexem lex, synt_T& parser);
lexem vardParse(lexem lex, synt_T& parser);
int   blockParse(lexem lex, synt_T& parser);
int   programParse(synt_T& parser);
void buildTreeStub(lexem lex) {
	std::cout << "<D> stub, get lexem " << lex.first << " (" << lex.second << ")"
			  << std::endl;
}
int expressionParse(lexem lex, synt_T& Parser) {
	lex = Parser.GetLex();
	switch (lex.second) {
	case id_tk:
	case constant_tk: {

		lex = Parser.PeekLex(1);
		do
		{
			if (lex.second == add_tk || lex.second == sub_tk || lex.second == mul_tk || lex.second == div_tk) {
				lex = Parser.GetLex();
				lex = Parser.GetLex();
				if (lex.second != id_tk && lex.second != constant_tk) {
					ErrMsg(Parser.GetCurrentLine(), lex, "Must be identificator or constant");
					std::cerr << "<G> Must be identificator or constant" << std::endl;
					return -EXIT_FAILURE;
				}
			}
			lex = Parser.PeekLex(1);
		} while (lex.second != semi_tk);
		buildTreeStub(lex); // Here is your Tree realization
		break;
	}
	default: {
		ErrMsg(Parser.GetCurrentLine(), lex, "Должен быть идентификатор или константа или ' - ' или '('");
		std::cerr << "<G> Должен быть идентификатор или константа или ' - ' или '('"<< std::endl;
		return -EXIT_FAILURE;
	}
	}

	return EXIT_SUCCESS;
}
int stateParse(lexem& lex, synt_T& parser) {
	lex = parser.GetLex();
	switch (lex.second) {
	case id_tk: { // a := b (assign part)

		if (VarList.VarSearch(lex.first) != 1) {
			ErrMsg(parser.GetCurrentLine(), lex, "необъявленный иднтификатор");
			std::cerr << "<G> необъявленный идентификатор" << std::endl;
			return -EXIT_FAILURE;
		}

		lex = parser.GetLex();
		if (lex.second != ass_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, ":= отсутствует");
			std::cerr << "<G> := отсутствует" << std::endl;
			return -EXIT_FAILURE;
		}

		expressionParse(lex, parser);

		lex = parser.GetLex();
		if (lex.second != semi_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, "; отсутствует");
			std::cerr << "<G> ;отсутствует" << std::endl;
			return -EXIT_FAILURE;
		}
		break;
	}
	case begin_tk: { // compound statements
		compoundParse(lex, parser);
		lex = parser.GetLex();
		if (lex.second != semi_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, "';' отсутствует");
			std::cerr << "<G> ';'отсутствует" << std::endl;
			return -EXIT_FAILURE;
		}
		break;
	}
	default: {

		break;
	}
	}

	return EXIT_SUCCESS;
}
int compoundParse(lexem lex, synt_T& parser) {
	static int compound_count = 0;
	compound_count++;
	while (lex.second != end_tk) {
		buildTreeStub(lex); // Here is your Tree realization
		if (parser.FileEOF()) {
			ErrMsg(parser.GetCurrentLine(), lex, "Каждое начало должно быть закрыто'end'");
			std::cerr << "<G> Каждое начало должно быть закрыто 'end'" << std::endl;
			return -EXIT_FAILURE;
		}
		stateParse(lex, parser);
	}

	if (compound_count == 1) {
		if (parser.PeekLex(1).second == unknown_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, "'.' отсутствует");
			std::cerr << "<G> '.' отсутствует" << std::endl;
			return -EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
lexem vardParse(lexem lex, synt_T& parser) {
	lex = parser.GetLex();
	lexem VarType;
	int i = 0;
	if (lex.second != id_tk) {
		ErrMsg(parser.GetCurrentLine(), lex, "Здесь должен быть идентификатор");
		std::cerr << "<G> Здесь должен быть идентификатор" << std::endl;
		return lex;
	}
	if (VarList.VarSearch(lex.first) != 1)
	{
		do
		{
			i++;
			VarType = parser.PeekLex(i + 1);
			if (VarType.second == type_tk)
				VarList.VarSave(lex.first, VarType.first);
		} while (VarType.second != semi_tk && VarType.second != type_tk);
	}
	else
		ErrMsg(parser.GetCurrentLine(), lex, "Этот идентификатор уже используется");
	std::cerr << "<G> Этот идентификатор уже используется" << std::endl;

	lex = parser.GetLex();
	if (lex.second == comma_tk)
		return vardParse(lex, parser); // Раскручивая стек обратно,
									   // будет возвращено последнее значение

	return lex;
}
int blockParse(lexem lex, synt_T& parser) {
	lex = parser.GetLex();
	switch (lex.second) { // var / begin
	case var_tk: {   // var a, b: integer;
		lex = vardParse(lex, parser);
		if (lex.second != ddt_tk)
			ErrMsg(parser.GetCurrentLine(), lex, ": отсутствует");
		std::cerr << "<G> : отсутствует" << std::endl;

		lex = parser.GetLex();
		if (lex.second != type_tk)
			ErrMsg(parser.GetCurrentLine(), lex, "Идентификатор должен иметь тип");
		std::cerr << "<G> Идентификатор должен иметь тип" << std::endl;

		lex = parser.GetLex();
		if (lex.second != semi_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, "; отсутствует");
			std::cerr << "<G> ; отсутствует" << std::endl;
			return -EXIT_FAILURE;
		}

		buildTreeStub(lex);

		break;
	}
	case begin_tk: {
		compoundParse(lex, parser);
		break;
	}
	case dot_tk: {
		std::cout << "программа была успешно разобрана" << std::endl;
		return 1;
	}
	default: {
		ErrMsg(parser.GetCurrentLine(), lex, "неизвестная лексема");
		std::cerr << "<E> неизвестная лексема" << std::endl;
		return -EXIT_FAILURE;
	}
	}

	return EXIT_SUCCESS;
}
int programParse(synt_T& parser) {
	auto lex = parser.GetLex();
	if (lex.second == program_tk) {
		lex = parser.GetLex();
		if (lex.second != id_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, "азвание программы отсутствует");
			std::cerr << "<G> название программы отсутствует" << std::endl;
			if (lex.second != semi_tk) {
				ErrMsg(parser.GetCurrentLine(), lex, "; отсутствует");
				std::cerr << "<G> ;отсутствует" << std::endl;
				return -EXIT_FAILURE;
			}
		}

		lex = parser.GetLex();
		if (lex.second != semi_tk) {
			ErrMsg(parser.GetCurrentLine(), lex, ";отсутствует");
			std::cerr << "<G> ; отсутствует" << std::endl;
			return -EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
	else {
		ErrMsg(parser.GetCurrentLine(), lex, "Неопределенный тип программы");
		std::cerr << "<G> Неопределенный тип программы" << std::endl;
		return -EXIT_FAILURE;
	}
}
int Parse2(const std::string& file_path) {
	try {
		synt_T example_synt;

		example_synt.FileOpen(file_path);
		if (!example_synt.FileIsOpen()) {
			ErrMsg("Неудается открыть файл");
			std::cerr << "<G> Неудается открыть файл" << std::endl;
			return -EXIT_FAILURE;
		}

		if (programParse(example_synt) != 0) {
			example_synt.FileClose();
			return -EXIT_FAILURE;
		}

		lexem lex;
		while (!example_synt.FileEOF() && !example_synt.FileFail()) {
			if (blockParse(lex, example_synt) == 1)
				break;
		}

		example_synt.FileClose();
		return EXIT_SUCCESS;
	}
	catch (const std::exception& exp) {
		std::cerr << "<G> Исключение в " << __func__ << ": " << exp.what()
			<< std::endl;
		return -EXIT_FAILURE;
	}
}
void  buildTreeStub(lexem lex);
int   expressionParse(lexem lex, synt_T& parser);
int   stateParse(lexem& lex, synt_T& parser);
int   compoundParse(lexem lex, synt_T& parser);
lexem vardParse(lexem lex, synt_T& parser);
int   blockParse(lexem lex, synt_T& parser);
int   programParse(synt_T& parser);