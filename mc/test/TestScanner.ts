import { TokenType } from '../src/TokenType'
import { Scanner } from '../src/Scanner'
import assert from 'assert'

describe('Scanner', function () {
    it('Simple Text', function () {
        let scanner: Scanner;
        let source: string;
        source = "Hello World";
        scanner = new Scanner(source);
        let token = scanner.Scan();
        assert(token.type == TokenType.TEXT);
        assert(source.substring(token.from, token.to) == "Hello World");
    })
});
