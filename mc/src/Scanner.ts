import { TokenType } from './TokenType'
import { Token } from './Token'

export class Scanner {

    private s: string;
    private p: number;
    private m: boolean;
    l: number;
    c: number;

    constructor(s: string) {
        this.s = s;
        this.p = 0;
        this.m = false;
        this.l = 1;
        this.c = 1;
    }

    private AdvancePosition(): void {
        if (this.s[this.p] == '\n') {
            this.l = this.l + 1;
            this.c = 1;
        } else {
            this.c = this.c + 1;
        }
        this.p = this.p + 1;
    }

    public Scan(): Token {
        let i: number;
        if (this.p == this.s.length) {
            return new Token(TokenType.EOF, 0, 0);
        } else {
            if (this.IsUnescaped('{')) {
                this.AdvancePosition();
                this.m = true;
                return new Token(TokenType.OPEN_BRACE, this.p - 1, this.p);
            } else if (this.IsUnescaped('}')) {
                this.AdvancePosition();
                this.m = false;
                return new Token(TokenType.CLOSE_BRACE, this.p - 1, this.p);
            } else if (this.s[this.p] == ':') {
                this.AdvancePosition();
                this.m = false;
                return new Token(TokenType.COLON, this.p - 1, this.p);
            } else {
                if (this.m) {
                    if (this.IsAlphabet(this.s[this.p])) {
                        i = this.p;
                        while (true) {
                            if (this.p == this.s.length) {
                                break;
                            } else if (this.IsAlphaNumeric(this.s[this.p])) {
                                this.AdvancePosition();
                            } else {
                                break;
                            }
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
                            if (this.IsUnescaped('{')) {
                                terminated = true;
                            } else {
                                this.AdvancePosition();
                            }
                        } else if (this.s[this.p] == '}') {
                            if (this.IsUnescaped('}')) {
                                terminated = true;
                            } else {
                                this.AdvancePosition();
                            }
                        }
                        if (terminated) {
                            return new Token(TokenType.TEXT, i, this.p);
                        }
                        this.AdvancePosition();
                    }
                }
            }
        }
    }

    private IsAlphabet(c: string): boolean {
        return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
    }

    private IsDigit(c: string): boolean {
        return ('0' <= c && c <= '9');
    }

    private IsAlphaNumeric(c: string): boolean {
        return this.IsAlphabet(c) || this.IsDigit(c);
    }

    private IsUnescaped(c: string): boolean {
        if (this.s[this.p] == c) {
            if (this.p + 1 == this.s.length) {
                return true;
            } else if (this.s[this.p + 1] != c) {
                return true;
            }
        }
        return false;
    }

    Unescape(from: number, to: number): string {
        let result: string;
        result = this.s.substring(from, to);
        result = result.replace("{{", "{");
        result = result.replace("}}", "}");
        return result;
    }
}