import { CellElement } from "./CellElement";
import { IDocumentElement } from "./IDocumentElement";
import { ParseResult } from "./ParseResult";
import { Scanner } from "./Scanner";
import { TextElement } from "./TextElement";
import { Token } from "./Token";
import { TokenType } from "./TokenType";

export class Parser {

    private s: Scanner;
    private t: Token;

    constructor(s: string) {
        this.s = new Scanner(s);
        this.t = this.s.Scan();
    }

    Parse(): ParseResult {
        let elements: Array<IDocumentElement>;
        let errors: Array<string>;
        let element: IDocumentElement | string;
        elements = [];
        errors = [];
        while (this.t.type != TokenType.EOF) {
            element = this.ParseElement();
            if (typeof element == "string") {
                errors.push(element as string);
            }
            else {
                elements.push(element as IDocumentElement);
            }
        }
        return new ParseResult(elements, errors);
    }

    private ParseElement(): IDocumentElement | string {
        let s: string;
        let t: string;
        if (this.t.type == TokenType.TEXT) {
            s = this.s.Unescape(this.t.from, this.t.to);
            this.t = this.s.Scan();
            return new TextElement(s);
        } else if (this.t.type == TokenType.OPEN_BRACE) {
            this.t = this.s.Scan();
            if (this.t.type == TokenType.ID) {
                s = this.s.Unescape(this.t.from, this.t.to);
                this.t = this.s.Scan();
                if (this.t.type == TokenType.COLON) {
                    this.t = this.s.Scan();
                    if (this.t.type == TokenType.TEXT) {
                        t = this.s.Unescape(this.t.from, this.t.to);
                        this.t = this.s.Scan();
                        if (this.t.type == TokenType.CLOSE_BRACE) {
                            this.t = this.s.Scan();
                            return new CellElement(s, t);
                        } else {
                            // TODO: Implement proper error reporting
                            throw "Error 1";
                        }
                    } else {
                        // TODO: Implement proper error reporting
                        throw "Error 2";
                    }
                } else if (this.t.type == TokenType.CLOSE_BRACE) {
                    this.t = this.s.Scan();
                    return new CellElement(s, undefined);
                } else {
                    // TODO: Implement proper error reporting
                    throw "Error 3";
                }
            } else {
                // TODO: Implement proper error reporting
                throw "Error 4";
            }
        } else {
            this.t = this.s.Scan();
            return "} unexpected at line " + this.s.l + " column " + (this.s.c - 1) + ".";
        }
    }
}