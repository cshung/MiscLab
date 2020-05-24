import { Compiler } from './Compiler'

(function () {
    let testInputArea: HTMLTextAreaElement;
    let testOutputDiv: HTMLDivElement;

    function BindEvents() {
        window.onload = OnWindowLoaded;
    }

    function OnWindowLoaded() {
        testInputArea = document.getElementById('TestInputArea') as HTMLTextAreaElement;
        testOutputDiv = document.getElementById('TestOutputDiv') as HTMLDivElement;
        testInputArea.onkeyup = OnTestButtonClicked;
    }

    function OnTestButtonClicked() {
        let compiler: Compiler = new Compiler();
        compiler.Compile(testInputArea.value);
        if (compiler.errors.length == 0) {
            testOutputDiv.innerHTML = compiler.element;
           eval(compiler.script);
        } else {
            testOutputDiv.innerHTML = "";
            for (let i = 0; i < compiler.errors.length; i++) {
                testOutputDiv.innerHTML += compiler.errors[i] + "<br/>";
            }
        }
    }

    BindEvents();
})();