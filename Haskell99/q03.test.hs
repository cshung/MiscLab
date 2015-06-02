import Test.HUnit
import Q03

assertEqualEither ::String -> Either Int String -> Either Int String -> Assertion
assertEqualEither = assertEqual

test1 = TestCase (assertEqualEither "elementAtTest [] 0    should be an error." (Right "k <= 0 is not supported") (elementAtTest [] 0    )) 
test2 = TestCase (assertEqualEither "elementAtTest [] -1   should be an error." (Right "k <= 0 is not supported") (elementAtTest [] (-1) ))
test3 = TestCase (assertEqualEither "elementAtTest [] 1    should be an error." (Right "Element not found")       (elementAtTest [] 1    ))
test4 = TestCase (assertEqualEither "elementAtTest [1] 2   should be an error." (Right "Element not found")       (elementAtTest [1] 2   ))
test5 = TestCase (assertEqualEither "elementAtTest [1,2] 1 should be 1. "       (Left 1)                          (elementAtTest [1,2] 1 ))
test6 = TestCase (assertEqualEither "elementAtTest [1,2] 2 should be 2. "       (Left 2)                          (elementAtTest [1,2] 2 ))

main = runTestTT $ TestList [test1, test2, test3, test4, test5, test6]