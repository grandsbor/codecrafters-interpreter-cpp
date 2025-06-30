#pragma once
#include <vector>
#include <tuple>
#include <set>

#include "token.h"

#define LEXER_ERR 65;

enum class TokenizerState {
	Default,
	StringLiteral,
	Number,
	Identifier,
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
		case TokenizerState::StringLiteral:
			LogError("Unterminated string.");
			RetCode_ = LEXER_ERR;
			break;
		case TokenizerState::Number:
			EndOfLiteral(TokenType::Number);
			break;
		case TokenizerState::Identifier:
			EndOfLiteral(TokenType::Identifier);
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
				EndOfLiteral(TokenType::String);
			} else if (c == '\n') {
				LogError("Unterminated string.");
				RetCode_ = LEXER_ERR;
			}
			break;
		case TokenizerState::Identifier:
			if (!IsNormalChar(c) && !IsDigit(c)) {
				EndOfLiteral(TokenType::Identifier);
				--Pos_;
			}
			break;
		case TokenizerState::Number:
			if (c == '.') {
				if (NumberHasPoint || !IsDigit(NextChar())) {
					EndOfLiteral(TokenType::Number);
					NumberHasPoint = false;
					--Pos_;
				} else {
					NumberHasPoint = true;
				}
			} else if (!IsDigit(c)) {
				EndOfLiteral(TokenType::Number);
				NumberHasPoint = false;
				--Pos_;
			}
			break;
		default:
			if (IsHalfChar(c) && NextChar() == '=') {
				Tokens_.emplace_back(c, '=', this->LineNo_);
				++Pos_;
			} else if (c == '/' && NextChar() == '/') {
				State_ = TokenizerState::Comment;
				++Pos_;
			} else if (c == '"') {
				State_ = TokenizerState::StringLiteral;
				LiteralStart_ = Pos_;
			} else if (IsNormalChar(c)) {
				State_ = TokenizerState::Identifier;
				LiteralStart_ = Pos_;
			} else if (IsDigit(c)) {
				State_ = TokenizerState::Number;
				LiteralStart_ = Pos_;
			} else if (!IsSpace(c)) {
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

	void EndOfLiteral(TokenType type) {
		size_t len = Pos_ - LiteralStart_ + (type == TokenType::String);
		Tokens_.emplace_back(
			type,
			this->LineNo_,
			Content_.substr(LiteralStart_, len)
		);
		State_ = TokenizerState::Default;
	}

	bool AtEnd() const {
		return Pos_ >= Content_.size(); 
	}

	static bool IsHalfChar(char c) {
		static const std::set<char> CHARS = {'=', '!', '<', '>'};
		return CHARS.contains(c);
	}

	static bool IsSpace(char c) {
		return c == ' ' || c == '\t' || c == '\n';
	}

	static bool IsDigit(char c) {
		return c >= '0' && c <= '9';
	}

	static bool IsNormalChar(char c) {
		return c == '_'
			|| (c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z');;
	}

	char NextChar() const {
		if (Pos_ >= Content_.size() - 1) {
			return 0;
		}
		return Content_[Pos_ + 1];
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
	bool NumberHasPoint = false;
};