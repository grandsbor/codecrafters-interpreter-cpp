#pragma once
#include <vector>
#include <tuple>

#include "token.h"

class Tokenizer {
public:
	Tokenizer(const std::string& s)
		: Content_(s)
	{}

	std::tuple<std::vector<Token>, int> Tokenize() {
		while (!AtEnd()) {
			Advance();
		}
		Tokens_.emplace_back(Token(TokenType::Eof, this->LineNo_));
		return {std::move(Tokens_), RetCode};
	}
private:
	void Advance() {
		char c = Content_[Pos++];
		try {
			Token t(c, this->LineNo_);
			Tokens_.push_back(std::move(t));
		} catch (const UnknownCharacterError&) {
			;
		}
	}

	bool AtEnd() const {
		return Pos >= Content_.size(); 
	}
private:
	const std::string& Content_;
	size_t LineNo_ = 1;
	size_t Pos = 0;
	int RetCode = 0;
	std::vector<Token> Tokens_;
};