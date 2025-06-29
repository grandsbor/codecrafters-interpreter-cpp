#pragma once
#include <vector>
#include <tuple>
#include <set>

#include "token.h"

#define LEXER_ERR 65;

enum class TokenizerState {
	Default,
	MaybePartialToken,
	StringLiteral,
	Comment,
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
		case TokenizerState::MaybePartialToken:
			Tokens_.emplace_back(PrevChar(), this->LineNo_);
			break;
		case TokenizerState::StringLiteral:
			LogError("Unterminated string.");
			RetCode_ = LEXER_ERR;
			break;
		}
		Tokens_.emplace_back(TokenType::Eof, this->LineNo_);
		return {std::move(Tokens_), RetCode_};
	}
private:
	void Advance() {
		char c = Content_[Pos_];
		switch (State_) {
		case TokenizerState::Comment:
			break;
		case TokenizerState::StringLiteral:
			if (c == '"') {
				Tokens_.emplace_back(
					TokenType::String,
					this->LineNo_,
					Content_.substr(LiteralStart_, Pos_ - LiteralStart_ + 1)
				);
				State_ = TokenizerState::Default;
			} else if (c == '\n') {
				LogError("Unterminated string.");
				RetCode_ = LEXER_ERR;
			}
			break;
		case TokenizerState::MaybePartialToken:
			if (c == '/' && PrevChar() == c) {
				State_ = TokenizerState::Comment;
				break;
			} else if (c == '=') {
				Tokens_.emplace_back(PrevChar(), c, this->LineNo_);
			} else {
				Tokens_.emplace_back(PrevChar(), this->LineNo_);
				--Pos_;
			}
			State_ = TokenizerState::Default;
			break;
		default:
			if (IsHalfChar(c)) {
				State_ = TokenizerState::MaybePartialToken;
			} else if (c == '"') {
				State_ = TokenizerState::StringLiteral;
				LiteralStart_ = Pos_;
			} else if (c != ' ' && c != '\t' && c != '\n') {
				try {
					Tokens_.emplace_back(c, this->LineNo_);
				} catch (const UnknownCharacterError&) {
					LogError(std::string("Unexpected character: ") + c);
					RetCode_ = LEXER_ERR;
				}
			}
		}
		if (c == '\n') {
			State_ = TokenizerState::Default;
			++LineNo_;
		}
		++Pos_;
	}

	bool AtEnd() const {
		return Pos_ >= Content_.size(); 
	}

	static bool IsHalfChar(char c) {
		static const std::set<char> CHARS = {'=', '!', '<', '>', '/'};
		return CHARS.contains(c);
	}

	char PrevChar() const {
		if (Pos_ == 0) {
			throw std::logic_error("");
		}
		return Content_[Pos_ - 1];
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
	size_t LiteralStart_ = 0;
};