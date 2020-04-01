import { IDocumentElement } from './IDocumentElement'

export class CellElement implements IDocumentElement {

    name: string;
    value?: string;

    constructor(name: string, value?: string) {
        this.name = name;
        this.value = value;
    }

    dummy() : void
    {
    }    
}