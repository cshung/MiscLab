import Q18
main = do
  s <- getContents
  let r = map generateScript (lines s)
  putStr (unlines r)

-- cannot believe I have to write this code :(
lastIndexOf s c = 
  let
    -- str/find/current/best
    lastIndexOf' [] c i m = m
    lastIndexOf' (x:xs) c i m = if (x == c) then lastIndexOf' xs c (i+1) i else lastIndexOf' xs c (i+1) m
  in            
    lastIndexOf' s c 1 1

grabGuid s = slice s ((lastIndexOf s '/')+1) ((lastIndexOf s '.')-1)

generateScript s = generate (grabGuid s)

generate s = 
  "pushd ..\\.." ++ "\n" ++
  "  git clone https://gist.github.com/" ++ s ++ ".git" ++ "\n" ++
  "  pushd " ++ s ++ "\n" ++
  "    for %%1 in (*.*) do copy /y ..\\MiscLab\\SuffixTree\\%%1" ++ "\n" ++
  "    git add ." ++ "\n" ++
  "    git commit -m \"Update\"" ++ "\n" ++
  "    git push" ++ "\n" ++
  "  popd" ++ "\n" ++
  "  rd /s/q " ++ s ++ "\n" ++
  "popd" ++ "\n"