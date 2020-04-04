import { IDocumentElement } from './IDocumentElement'

export class CellElement implements IDocumentElement {

    name: string;
    value?: string;
    startLine?: number;
    startColumn?: number;
    endLine?: number;
    endColumn?: number;

    constructor(name: string, value?: string, startLine?: number, startColumn?: number, endLine?: number, endColumn?: number) {
        this.name = name;
        this.value = value;
        this.startLine = startLine;
        this.startColumn = startColumn;
        this.endLine = endLine;
        this.endColumn = endColumn;
    }

    dummy(): void {
    }
}