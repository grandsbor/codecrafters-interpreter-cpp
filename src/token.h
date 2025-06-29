#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

class UnknownCharacterError: public std::exception {
};

enum class TokenType {
	Eof,
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Comma,
	Dot,
	Minus,
	Plus,
	Semicolon,
	Star,
	Slash,
};

inline const std::unordered_map<TokenType, std::string> TS = {
	{TokenType::Eof, "EOF"},
	{TokenType::LeftParen, "LEFT_PAREN"},
	{TokenType::RightParen, "RIGHT_PAREN"},
	{TokenType::LeftBrace, "LEFT_BRACE"},
	{TokenType::RightBrace, "RIGHT_BRACE"},
	{TokenType::Comma, "COMMA"},
	{TokenType::Dot, "DOT"},
	{TokenType::Minus, "MINUS"},
	{TokenType::Plus, "PLUS"},
	{TokenType::Semicolon, "SEMICOLON"},
	{TokenType::Star, "STAR"},
};

std::ostream& operator<<(std::ostream& os, TokenType type) {
	auto it = TS.find(type);
	if (it != TS.end()) {
		os << it->second;
	}
	return os;
}

struct Token {
	TokenType Type_;
	std::string Lexeme_;
	std::string Literal_;
	size_t LineNo_;

	Token(TokenType type, size_t lineNo, std::string lexeme = {})
		: Type_(type)
		, LineNo_(lineNo)
		, Lexeme_(lexeme)
	{}
	
	Token(char c, size_t lineNo) {
		switch (c) {
		case '(': Type_ = TokenType::LeftParen; break;
		case ')': Type_ = TokenType::RightParen; break;
		case '{': Type_ = TokenType::LeftBrace; break;
		case '}': Type_ = TokenType::RightBrace; break;
		case ',': Type_ = TokenType::Comma; break;
		case '.': Type_ = TokenType::Dot; break;
		case '-': Type_ = TokenType::Minus; break;
		case '+': Type_ = TokenType::Plus; break;
		case ';': Type_ = TokenType::Semicolon; break;
		case '*': Type_ = TokenType::Star; break;
		default:
			throw UnknownCharacterError();
		}

		LineNo_ = lineNo;
		Lexeme_ = c;
	}

	std::string Str() const {
		std::ostringstream ss;
		ss << Type_ << ' ' << Lexeme_ << ' ' << (Literal_.empty() ? "null" : Literal_);
		return ss.str();
	}
};