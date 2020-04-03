import { Parser } from "./Parser";
import { ParseResult } from "./ParseResult";
import { CellElement } from "./CellElement";

export class Compiler {

    errors: Array<string>;

    constructor() {
        this.errors = [];
    }

    compile(s: string) {
        let parser: Parser;
        let result: ParseResult;
        let cells: { [name: string]: CellElement; }
        let cell: CellElement;
        let key: string;
        let i: number;

        parser = new Parser(s);
        result = parser.Parse();
        if (result.errors.length != 0) {
            for (i = 0; i < result.errors.length; i++) {
                this.errors.push(result.errors[i]);
            }
            return;
        }
        cells = {};
        for (i = 0; i < result.elements.length; i++) {
            if (result.elements[i] instanceof CellElement) {
                cell = result.elements[i] as CellElement;
                if (cells.hasOwnProperty(cell.name)) {
                    this.errors.push("TODO: Proper error message");
                }
                cells[cell.name] = cell;
            }
        }
        for (key in cells)
        {
            cell = cells[key];
            if (cell.value != undefined) {
                // TODO: Find references
            }
        }
        if (this.errors.length != 0) {
            return;
        }
    }
}