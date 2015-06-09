import Test.HUnit
import Q33

test1 = TestCase (assertEqual "tryCoPrime 0  15 should be Right Zero is not supported." (Right "Zero is not supported") (tryCoPrime 0  15))
test2 = TestCase (assertEqual "tryCoPrime 15 0  should be Right Zero is not supported." (Right "Zero is not supported") (tryCoPrime 15 0 ))
test3 = TestCase (assertEqual "tryCoPrime 12 32 should be Left False                 ." (Left False                   ) (tryCoPrime 12 32))
test4 = TestCase (assertEqual "tryCoPrime 31 12 should be Left True                  ." (Left True                    ) (tryCoPrime 31 12))

main = runTestTT $ TestList [test1,test2,test3,test4]