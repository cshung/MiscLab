import { CellElement } from '../src/CellElement';
import { IDocumentElement } from '../src/IDocumentElement';
import { Parser } from '../src/Parser'
import { TextElement } from '../src/TextElement';
import { Token } from 'Token';
import { TokenType } from '../src/TokenType'
import assert from 'assert'

describe('Parser', function () {

    function TestParser(document: string, expect: Array<IDocumentElement>) {
        let parser: Parser;
        let actual: Array<IDocumentElement>;
        let i: number;
        parser = new Parser(document);
        actual = parser.Parse();
        assert(actual.length == expect.length);
        for (i = 0; i < actual.length; i++) {
            assert(Equals(actual[i], expect[i]));
        }
    }

    function Equals(left: IDocumentElement, right: IDocumentElement): boolean {
        if ((left instanceof TextElement) && (right instanceof TextElement)) {
            return (left as TextElement).s == (right as TextElement).s;
        } else if ((left instanceof CellElement) && (right instanceof CellElement)) {
            return ((left as CellElement).name == (right as CellElement).name) && ((left as CellElement).value == (right as CellElement).value);
        }
        return false;
    }

    it('Empty String', function () {
        TestParser("", []);
    });
    it('Simple Text', function () {
        TestParser("Hello World", [new TextElement("Hello World")]);
    });
    it('Simple Cell', function () {
        TestParser("{a}", [new CellElement("a", undefined)]);
    });
    it('Complex Cell', function () {
        TestParser("{a:1}", [new CellElement("a", "1")]);
    });
    it('Example', function () {
        TestParser("The sum of {a} and {b} is {c:a+b}", [
            new TextElement("The sum of "),
            new CellElement("a", undefined),
            new TextElement(" and "),
            new CellElement("b", undefined),
            new TextElement(" is "),
            new CellElement("c", "a+b")
        ]);
    });
});
