import Test.HUnit
import Q21

test1 = TestCase (assertEqual "insertAtTest 0 [1,2] 0 should be Right r < 1 is not supported         ." (Right "r < 1 is not supported"         ) (insertAtTest 0 [1,2] 0))
test2 = TestCase (assertEqual "insertAtTest 0 [1,2] 1 should be Left [0,1,2]                         ." (Left [0,1,2]                           ) (insertAtTest 0 [1,2] 1))
test3 = TestCase (assertEqual "insertAtTest 0 [1,2] 2 should be Left [1,0,2]                         ." (Left [1,0,2]                           ) (insertAtTest 0 [1,2] 2))
test4 = TestCase (assertEqual "insertAtTest 0 [1,2] 3 should be Left [1,2,0]                         ." (Left [1,2,0]                           ) (insertAtTest 0 [1,2] 3))
test5 = TestCase (assertEqual "insertAtTest 0 [1,2] 4 should be Right r > length + 1 is not supported." (Right "r > length + 1 is not supported") (insertAtTest 0 [1,2] 4))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]