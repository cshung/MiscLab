import { Compiler } from './Compiler'

(function () {
    let testButton: HTMLButtonElement;
    let testInputArea: HTMLTextAreaElement;
    let testOutputArea: HTMLTextAreaElement;

    function BindEvents() {
        window.onload = OnWindowLoaded;
    }

    function OnWindowLoaded() {
        testButton = document.getElementById('TestButton') as HTMLButtonElement;
        testInputArea = document.getElementById('TestInputArea') as HTMLTextAreaElement;
        testOutputArea = document.getElementById('TestOutputArea') as HTMLTextAreaElement;
        testButton.onclick = OnTestButtonClicked;
    }

    function OnTestButtonClicked() {
        let compiler: Compiler = new Compiler();
        compiler.Compile(testInputArea.value);
        if (compiler.errors.length == 0) {
            testOutputArea.value = compiler.output;
        } else {
            testOutputArea.value = "";
            for (let i = 0; i < compiler.errors.length; i++) {
                testOutputArea.value += compiler.errors[i] + "\n";
            }
        }
    }

    BindEvents();
})();