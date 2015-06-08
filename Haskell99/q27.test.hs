import Test.HUnit
import Q27

test1 = TestCase (assertEqual "group []    [1,2,3] should be [[]]                                 ." ([[]]                                 ) (group []    [1,2,3]))
test2 = TestCase (assertEqual "group [1]   [1,2,3] should be [[[1]],[[2]],[[3]]]                  ." ([[[1]],[[2]],[[3]]]                  ) (group [1]   [1,2,3]))
test3 = TestCase (assertEqual "group [1,2] [1,2,3] should be [[[1],[2,3]],[[2],[1,3]],[[3],[1,2]]]." ([[[1],[2,3]],[[2],[1,3]],[[3],[1,2]]]) (group [1,2] [1,2,3]))

main = runTestTT $ TestList [test1,test2,test3]