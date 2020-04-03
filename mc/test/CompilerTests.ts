import { CellElement } from '../src/CellElement';
import { IDocumentElement } from '../src/IDocumentElement';
import { Compiler } from '../src/Compiler'
import { TextElement } from '../src/TextElement';
import { Token } from 'Token';
import { TokenType } from '../src/TokenType'
import assert from 'assert'

describe('Compiler', function () {

    function TestCompiler(s: string, errors: Array<string>) {
        let compiler: Compiler;
        let i: number;
        compiler = new Compiler();
        compiler.compile(s);
        assert(compiler.errors.length == errors.length);
        for (i = 0; i < errors.length; i++) {
            assert(compiler.errors[i] == errors[i]);
        }
    }

    it('ParseError', function () {
        TestCompiler("}", ["} unexpected at line 1 column 1."]);
    });

    it('DuplicateId', function () {
        TestCompiler("{a}{a}", ["TODO: Proper error message"]);
    });
});
