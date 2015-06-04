import Test.HUnit
import Q20

test1 = TestCase (assertEqual "removeAtTest [1,2,3] 0 should be Right q < 1 is not supported     ." (Right "q < 1 is not supported"     ) (removeAtTest [1,2,3] 0))
test2 = TestCase (assertEqual "removeAtTest [1,2,3] 1 should be Left (1,[2,3])                   ." (Left (1,[2,3])                     ) (removeAtTest [1,2,3] 1))
test3 = TestCase (assertEqual "removeAtTest [1,2,3] 2 should be Left (2,[1,3])                   ." (Left (2,[1,3])                     ) (removeAtTest [1,2,3] 2))
test4 = TestCase (assertEqual "removeAtTest [1,2,3] 3 should be Left (3,[1,2])                   ." (Left (3,[1,2])                     ) (removeAtTest [1,2,3] 3))
test5 = TestCase (assertEqual "removeAtTest [1,2,3] 4 should be Right q > length is not supported." (Right "q > length is not supported") (removeAtTest [1,2,3] 4))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]