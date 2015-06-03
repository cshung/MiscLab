import Test.HUnit
import Q11

assertEqualEntryList :: String -> [Entry Int] -> [Entry Int] -> Assertion
assertEqualEntryList = assertEqual

test1 = TestCase (assertEqualEntryList "encodeModified []      should be []                                ." ([]                                ) (encodeModified []     ))
test2 = TestCase (assertEqualEntryList "encodeModified [1]     should be [(Single 1)]                      ." ([(Single 1)]                      ) (encodeModified [1]    ))
test3 = TestCase (assertEqualEntryList "encodeModified [1,2]   should be [(Single 1),(Single 2)]           ." ([(Single 1),(Single 2)]           ) (encodeModified [1,2]  ))
test4 = TestCase (assertEqualEntryList "encodeModified [1,2,1] should be [(Single 1),(Single 2),(Single 1)]." ([(Single 1),(Single 2),(Single 1)]) (encodeModified [1,2,1]))
test5 = TestCase (assertEqualEntryList "encodeModified [1,1,2] should be [(Multiple 2 1),(Single 2)]       ." ([(Multiple 2 1),(Single 2)]       ) (encodeModified [1,1,2]))
test6 = TestCase (assertEqualEntryList "encodeModified [1,2,2] should be [(Single 1),(Multiple 2 2)]       ." ([(Single 1),(Multiple 2 2)]       ) (encodeModified [1,2,2]))
                                                                                  
main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6]                 
                                                                                  
                                                                                  
                                                                                  