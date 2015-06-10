import Test.HUnit
import Q40

test1 = TestCase (assertEqual "tryGoldbach (-1) should be ." (Right "Nonpositive integer is not supported") (tryGoldbach (-1)))
test2 = TestCase (assertEqual "tryGoldbach 0    should be ." (Right "Nonpositive integer is not supported") (tryGoldbach 0   ))
test3 = TestCase (assertEqual "tryGoldbach 1    should be ." (Right "Odd integer is not supported"        ) (tryGoldbach 1   ))
test4 = TestCase (assertEqual "tryGoldbach 2    should be ." (Right "2 is not supported"                  ) (tryGoldbach 2   ))
test5 = TestCase (assertEqual "tryGoldbach 10   should be ." (Left (3,7)                                  ) (tryGoldbach 10  ))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]