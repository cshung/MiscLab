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
        if (this.p == this.s.length) {
            return new Token(TokenType.EOF, 0, 0);
        } else if (this.m) {
            // TODO: Implementation
            return new Token(TokenType.EOF, 0, 0);
        } else {
            if (this.s[this.p] == '{') {
                this.p = this.p + 1;
                return new Token(TokenType.OPEN_BRACE, this.p - 1, this.p);
            } else if (this.s[this.p] == '}') {
                this.p = this.p + 1;
                return new Token(TokenType.CLOSE_BRACE, this.p - 1, this.p);
            } else {
                let i = this.p;
                while (true) {
                    this.p = this.p + 1;
                    let terminated = false;
                    if (this.p == this.s.length) {
                        terminated = true;
                    } else if (this.s[this.p] == '{') {
                        if (this.p + 1 < this.s.length && this.s[this.p + 1] == '{') {
                            this.p = this.p + 1;
                        } else {
                            terminated = true;
                        }
                    }
                    if (terminated) {
                        return new Token(TokenType.TEXT, i, this.p);
                    }
                }
            }
        }
    }
}