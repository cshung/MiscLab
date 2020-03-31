import { TokenType } from './TokenType'
import { Token } from './Token'

export class Scanner {
    private s: string;
    private p: number;
    private m: boolean;
    constructor(s: string) {
        this.s = s;
        this.p = 0;
        this.m = false;
    }
    public Scan(): Token {
        let i: number;
        if (this.p == this.s.length) {
            return new Token(TokenType.EOF, 0, 0);
        } else {
            if (this.isUnescaped('{')) {
                this.p = this.p + 1;
                this.m = true;
                return new Token(TokenType.OPEN_BRACE, this.p - 1, this.p);
            } else if (this.isUnescaped('}')) {
                this.p = this.p + 1;
                this.m = false;
                return new Token(TokenType.CLOSE_BRACE, this.p - 1, this.p);
            } else if (this.s[this.p] == ':') {
                this.p = this.p + 1;
                this.m = false;
                return new Token(TokenType.COLON, this.p - 1, this.p);
            } else {
                if (this.m) {
                    if (this.isAlphabet(this.s[this.p])) {
                        i = this.p;
                        while (this.isAlphaNumeric(this.s[this.p])) {
                            this.p = this.p + 1;
                        }
                        return new Token(TokenType.ID, i, this.p);
                    }
                    return new Token(TokenType.EOF, 0, 0);
                } else {
                    i = this.p;
                    while (true) {
                        let terminated = false;
                        if (this.p == this.s.length) {
                            terminated = true;
                        } else if (this.s[this.p] == '{') {
                            if (this.isUnescaped('{')) {
                                terminated = true;
                            } else {
                                this.p = this.p + 1;
                            }                        
                        } else if (this.s[this.p] == '}') {
                            if (this.isUnescaped('}')) {
                                terminated = true;
                            } else {
                                this.p = this.p + 1;
                            }
                        }
                        if (terminated) {
                            return new Token(TokenType.TEXT, i, this.p);
                        }
                        this.p = this.p + 1;
                    }
                }
            }
        }
    }
    private isAlphabet(c: string): boolean {
        return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
    }
    private isDigit(c: string): boolean {
        return ('0' <= c && c <= '9');
    }
    private isAlphaNumeric(c: string): boolean {
        return this.isAlphabet(c) || this.isDigit(c);
    }
    private isUnescaped(c: string): boolean {
        if (this.s[this.p] == c) {
            if (this.p + 1 == this.s.length) {
                return true;
            } else if (this.s[this.p + 1] != c) {
                return true;
            }
        }
        return false;
    }
}