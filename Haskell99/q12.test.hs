import Test.HUnit
import Q11
import Q12

assertEqualIntList :: String -> [Int] -> [Int] -> Assertion
assertEqualIntList = assertEqual

test1 = TestCase (assertEqualIntList "decodeModified []                                 should be []     ." ([]     ) (decodeModified []                                ))
test2 = TestCase (assertEqualIntList "decodeModified [(Single 1)]                       should be [1]    ." ([1]    ) (decodeModified [(Single 1)]                      ))
test3 = TestCase (assertEqualIntList "decodeModified [(Single 1),(Single 2)]            should be [1,2]  ." ([1,2]  ) (decodeModified [(Single 1),(Single 2)]           ))
test4 = TestCase (assertEqualIntList "decodeModified [(Single 1),(Single 2),(Single 1)] should be [1,2,1]." ([1,2,1]) (decodeModified [(Single 1),(Single 2),(Single 1)]))
test5 = TestCase (assertEqualIntList "decodeModified [(Multiple 2 1),(Single 2)]        should be [1,1,2]." ([1,1,2]) (decodeModified [(Multiple 2 1),(Single 2)]       ))
test6 = TestCase (assertEqualIntList "decodeModified [(Single 1),(Multiple 2 2)]        should be [1,2,2]." ([1,2,2]) (decodeModified [(Single 1),(Multiple 2 2)]       ))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]