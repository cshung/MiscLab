import Test.HUnit
import Q26

test1 = TestCase (assertEqual "combine [1,2,3] 0 should be []                 ." ([]                 ) (combine [1,2,3] 0))
test2 = TestCase (assertEqual "combine [1,2,3] 1 should be [[1],[2],[3]]      ." ([[1],[2],[3]]      ) (combine [1,2,3] 1))
test3 = TestCase (assertEqual "combine [1,2,3] 2 should be [[1,2],[1,3],[2,3]]." ([[1,2],[1,3],[2,3]]) (combine [1,2,3] 2))
test4 = TestCase (assertEqual "combine [1,2,3] 3 should be [[1,2,3]]          ." ([[1,2,3]]          ) (combine [1,2,3] 3))
test5 = TestCase (assertEqual "combine [1,2,3] 4 should be []                 ." ([]                 ) (combine [1,2,3] 4))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]