import { Scanner } from "./Scanner";
import { IDocumentElement } from "./IDocumentElement";
import { Token } from "./Token";
import { TokenType } from "./TokenType";
import { TextElement } from "./TextElement";
import { CellElement } from "./CellElement";

export class Parser {

    private d: string;
    private s: Scanner;
    private t: Token;

    constructor(d: string) {
        this.d = d;
        this.s = new Scanner(d);
        this.t = this.s.Scan();
    }

    Parse(): Array<IDocumentElement> {
        let result: Array<IDocumentElement>;
        result = [];
        while (this.t.type != TokenType.EOF) {
            result.push(this.ParseElement());
        }
        return result;
    }

    ParseElement(): IDocumentElement {
        let s: string;
        let t: string;
        if (this.t.type == TokenType.TEXT) {
            s = this.d.substring(this.t.from, this.t.to);
            this.t = this.s.Scan();
            return new TextElement(s);
        } else if (this.t.type == TokenType.OPEN_BRACE) {
            this.t = this.s.Scan();
            if (this.t.type == TokenType.ID) {
                s = this.d.substring(this.t.from, this.t.to);
                this.t = this.s.Scan();
                if (this.t.type == TokenType.COLON) {
                    this.t = this.s.Scan();
                    if (this.t.type == TokenType.TEXT) {
                        t = this.d.substring(this.t.from, this.t.to);
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
            // TODO: Implement proper error reporting
            throw "Error 5"
        }
    }
}