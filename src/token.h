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
	Equal,
	EqualEqual,
	Bang,
	BangEqual,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	String,
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
	{TokenType::Slash, "SLASH"},
	{TokenType::Equal, "EQUAL"},
	{TokenType::EqualEqual, "EQUAL_EQUAL"},
	{TokenType::Bang, "BANG"},
	{TokenType::BangEqual, "BANG_EQUAL"},
	{TokenType::Less, "LESS"},
	{TokenType::LessEqual, "LESS_EQUAL"},
	{TokenType::Greater, "GREATER"},
	{TokenType::GreaterEqual, "GREATER_EQUAL"},
	{TokenType::String, "STRING"},
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
		, Lexeme_(std::move(lexeme))
	{
		if (type == TokenType::String && Lexeme_.size() > 2) {
			Literal_ = Lexeme_.substr(1, Lexeme_.size() - 2);
		}
	}
	
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
		case '=': Type_ = TokenType::Equal; break;
		case '!': Type_ = TokenType::Bang; break;
		case '<': Type_ = TokenType::Less; break;
		case '>': Type_ = TokenType::Greater; break;
		case '/': Type_ = TokenType::Slash; break;
		default:
			throw UnknownCharacterError();
		}

		LineNo_ = lineNo;
		Lexeme_ = c;
	}

	Token(char c1, char c2, size_t lineNo) {
		if (c2 != '=') {
			throw std::logic_error("Not implemented");
		}
		switch (c1) {
		case '=':
			Type_ = TokenType::EqualEqual;
			break;
		case '!':
			Type_ = TokenType::BangEqual;
			break;
		case '<':
			Type_ = TokenType::LessEqual;
			break;
		case '>':
			Type_ = TokenType::GreaterEqual;
			break;
		default:
			;
		}

		LineNo_ = lineNo;
		Lexeme_ = std::string{c1} + c2;
	}

	std::string Str() const {
		std::ostringstream ss;
		ss << Type_ << ' ' << Lexeme_ << ' ' << (Literal_.empty() ? "null" : Literal_);
		return ss.str();
	}
};