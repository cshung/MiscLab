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
        let cell2: CellElement;
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
                    cell2 = cells[cell.name];
                    this.errors.push(`Cell at (${cell.startLine}, ${cell.startColumn}) - (${cell.endLine}, ${cell.endColumn}) with name '${cell.name}' already defined at (${cell2.startLine}, ${cell2.startColumn}) - (${cell2.endLine}, ${cell2.endColumn})`);
                }
                cells[cell.name] = cell;
            }
        }
        for (key in cells)
        {
            cell = cells[key];
            if (cell.value != undefined) {
                
            }
        }
        if (this.errors.length != 0) {
            return;
        }
    }
}