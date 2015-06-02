import Test.HUnit
import Q02

assertEqualEither::String -> Either Int String -> Either Int String -> Assertion
assertEqualEither = assertEqual

test1 = TestCase (assertEqualEither "myButLastTest []        should an error. " (Right "Cannot obtain myButLast of an empty list")           (myButLastTest []       )) 
test2 = TestCase (assertEqualEither "myButLastTest [0]       should an error. " (Right "Cannot obtain myButLast of a list with one element") (myButLastTest [0]      ))
test3 = TestCase (assertEqualEither "myButLastTest [0, 1]    should be 0.     " (Left 0)                                                     (myButLastTest [0, 1]   ))
test4 = TestCase (assertEqualEither "myButLastTest [0, 1, 2] should be 1.     " (Left 1)                                                     (myButLastTest [0, 1, 2]))

main = runTestTT $ TestList [test1, test2, test3, test4]