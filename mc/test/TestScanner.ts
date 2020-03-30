import { TokenType } from '../src/TokenType'
import { Scanner } from '../src/Scanner'
import assert from 'assert'

describe('Scanner', function () {
    it('Empty String', function () {
        let scanner: Scanner;
        let source: string;
        source = "";
        scanner = new Scanner(source);
        let token = scanner.Scan();
        assert(token.type == TokenType.EOF);
    });
    it('Simple Text', function () {
        let scanner: Scanner;
        let source: string;
        source = "Hello World";
        scanner = new Scanner(source);
        let token = scanner.Scan();
        assert(token.type == TokenType.TEXT);
        assert(source.substring(token.from, token.to) == "Hello World");
        token = scanner.Scan();
        assert(token.type == TokenType.EOF);        
    });
    it('Open Brace', function () {
        let scanner: Scanner;
        let source: string;
        source = "{";
        scanner = new Scanner(source);
        let token = scanner.Scan();
        assert(token.type == TokenType.OPEN_BRACE);
        assert(source.substring(token.from, token.to) == "{");
    });
    it('Close Brace', function () {
        let scanner: Scanner;
        let source: string;
        source = "}";
        scanner = new Scanner(source);
        let token = scanner.Scan();
        assert(token.type == TokenType.CLOSE_BRACE);
        assert(source.substring(token.from, token.to) == "}");
    });
});
