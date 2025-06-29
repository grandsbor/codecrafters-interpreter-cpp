#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

enum class TokenType {
	Eof,
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
};

inline const std::unordered_map<TokenType, std::string> TS = {
	{TokenType::Eof, "EOF"},
	{TokenType::LeftParen, "LEFT_PAREN"},
	{TokenType::RightParen, "RIGHT_PAREN"},
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
		default:
			;
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