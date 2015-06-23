pushd ..\..
  git clone https://gist.github.com/dca239969747d75d03a1.git
  pushd dca239969747d75d03a1
    for %%1 in (*.*) do copy /y ..\MiscLab\SuffixTree\%%1
    git add .
    git commit -m "Update"
    git push
  popd
  rd /s/q dca239969747d75d03a1
popd

pushd ..\..
  git clone https://gist.github.com/4fac316cc07dfb83154f.git
  pushd 4fac316cc07dfb83154f
    for %%1 in (*.*) do copy /y ..\MiscLab\SuffixTree\%%1
    git add .
    git commit -m "Update"
    git push
  popd
  rd /s/q 4fac316cc07dfb83154f
popd

