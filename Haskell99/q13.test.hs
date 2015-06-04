import Test.HUnit
import Q13

assertEqualEntryList :: String -> [Entry Int] -> [Entry Int] -> Assertion
assertEqualEntryList = assertEqual

test1 = TestCase (assertEqualEntryList "encodeDirect []      should be []                                ." ([]                                ) (encodeDirect []     ))
test2 = TestCase (assertEqualEntryList "encodeDirect [1]     should be [(Single 1)]                      ." ([(Single 1)]                      ) (encodeDirect [1]    ))
test3 = TestCase (assertEqualEntryList "encodeDirect [1,2]   should be [(Single 1),(Single 2)]           ." ([(Single 1),(Single 2)]           ) (encodeDirect [1,2]  ))
test4 = TestCase (assertEqualEntryList "encodeDirect [1,2,1] should be [(Single 1),(Single 2),(Single 1)]." ([(Single 1),(Single 2),(Single 1)]) (encodeDirect [1,2,1]))
test5 = TestCase (assertEqualEntryList "encodeDirect [1,1,2] should be [(Multiple 2 1),(Single 2)]       ." ([(Multiple 2 1),(Single 2)]       ) (encodeDirect [1,1,2]))
test6 = TestCase (assertEqualEntryList "encodeDirect [1,2,2] should be [(Single 1),(Multiple 2 2)]       ." ([(Single 1),(Multiple 2 2)]       ) (encodeDirect [1,2,2]))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]