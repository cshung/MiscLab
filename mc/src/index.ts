import { Scanner } from './Scanner'
import { Token } from './Token';

(function () {
    let testButton: HTMLButtonElement;
    let testTextArea: HTMLTextAreaElement;

    function BindEvents() {
        window.onload = OnWindowLoaded;
    }

    function OnWindowLoaded() {
        testButton = document.getElementById('TestButton') as HTMLButtonElement;    
        testTextArea = document.getElementById('TestTextArea') as HTMLTextAreaElement;
        testButton.onclick = OnTestButtonClicked;
    }

    function OnTestButtonClicked() {
        let document: string;
        let scanner: Scanner;
        let token: Token;
        document = testTextArea.value;
        scanner = new Scanner(document);
        token = scanner.Scan();
        alert(document.substring(token.from, token.to));
    }

    BindEvents();
})();