import Test.HUnit
import Q10

assertEqualIntTupleList :: String -> [(Int,Int)] -> [(Int,Int)] -> Assertion
assertEqualIntTupleList = assertEqual

test1 = TestCase (assertEqualIntTupleList "encode []      should be []."                  ([]                 ) (encode []     ))
test2 = TestCase (assertEqualIntTupleList "encode [1]     should be [(1,1)]."             ([(1,1)]            ) (encode [1]    ))
test3 = TestCase (assertEqualIntTupleList "encode [1,2]   should be [(1,1),(2,1)]."       ([(1,1),(2,1)]      ) (encode [1,2]  ))
test4 = TestCase (assertEqualIntTupleList "encode [1,2,1] should be [(1,1),(2,1),(1,1)]." ([(1,1),(2,1),(1,1)]) (encode [1,2,1]))
test5 = TestCase (assertEqualIntTupleList "encode [1,1,2] should be [(1,2),(2,1)]."       ([(1,2),(2,1)]      ) (encode [1,1,2]))
test6 = TestCase (assertEqualIntTupleList "encode [1,2,2] should be [(1,1),(2,2)]."       ([(1,1),(2,2)]      ) (encode [1,2,2]))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]