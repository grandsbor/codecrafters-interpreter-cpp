#pragma once
#include <vector>
#include <tuple>

#include "token.h"

#define LEXERR_UNKNOWN_CHAR 65;

enum class TokenizerState {
	Default,
	AfterEqualSign,
};

class Tokenizer {
public:
	Tokenizer(const std::string& s)
		: Content_(s)
	{}

	std::tuple<std::vector<Token>, int> Tokenize() {
		while (!AtEnd()) {
			Advance();
		}
		switch (State_) {
		case TokenizerState::AfterEqualSign:
			Tokens_.emplace_back(TokenType::Equal, this->LineNo_);
			break;
		}
		Tokens_.emplace_back(TokenType::Eof, this->LineNo_);
		return {std::move(Tokens_), RetCode_};
	}
private:
	void Advance() {
		char c = Content_[Pos_++];
		switch (State_) {
		case TokenizerState::AfterEqualSign:
			if (c == '=') {
				Tokens_.emplace_back(TokenType::EqualEqual, this->LineNo_);
			} else {
				Tokens_.emplace_back(TokenType::Equal, this->LineNo_);
				--Pos_;
			}
			State_ = TokenizerState::Default;
			break;
		default:
			if (c == '=') {
				State_ = TokenizerState::AfterEqualSign;
				return;
			}
			try {
				Tokens_.emplace_back(c, this->LineNo_);
			} catch (const UnknownCharacterError&) {
				LogError(std::string("Unexpected character: ") + c);
				RetCode_ = LEXERR_UNKNOWN_CHAR;
			}
		}
	}

	bool AtEnd() const {
		return Pos_ >= Content_.size(); 
	}

	void LogError(const std::string& msg) const {
		std::cerr << "[line " << LineNo_ << "] Error: " << msg << std::endl;
	}
private:
	const std::string& Content_;
	size_t LineNo_ = 1;
	size_t Pos_ = 0;
	int RetCode_ = 0;
	TokenizerState State_ = TokenizerState::Default;
	std::vector<Token> Tokens_;
};