import Test.HUnit
import Q31

test01 = TestCase (assertEqual "tryIsPrime 1    should be Left True                                 ." (Left True                                   ) (tryIsPrime 1   ))
test02 = TestCase (assertEqual "tryIsPrime 2    should be Left True                                 ." (Left True                                   ) (tryIsPrime 2   ))
test03 = TestCase (assertEqual "tryIsPrime 3    should be Left True                                 ." (Left True                                   ) (tryIsPrime 3   ))
test04 = TestCase (assertEqual "tryIsPrime 4    should be Left False                                ." (Left False                                  ) (tryIsPrime 4   ))
test05 = TestCase (assertEqual "tryIsPrime 5    should be Left True                                 ." (Left True                                   ) (tryIsPrime 5   ))
test06 = TestCase (assertEqual "tryIsPrime 6    should be Left False                                ." (Left False                                  ) (tryIsPrime 6   ))
test07 = TestCase (assertEqual "tryIsPrime 7    should be Left True                                 ." (Left True                                   ) (tryIsPrime 7   ))
test08 = TestCase (assertEqual "tryIsPrime 8    should be Left False                                ." (Left False                                  ) (tryIsPrime 8   ))
test09 = TestCase (assertEqual "tryIsPrime 9    should be Left False                                ." (Left False                                  ) (tryIsPrime 9   ))
test10 = TestCase (assertEqual "tryIsPrime 10   should be Left False                                ." (Left False                                  ) (tryIsPrime 10  ))
test11 = TestCase (assertEqual "tryIsPrime 0    should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryIsPrime 0   ))
test12 = TestCase (assertEqual "tryIsPrime (-1) should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryIsPrime (-1)))
test13 = TestCase (assertEqual "tryIsPrime (-2) should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryIsPrime (-2)))

main = runTestTT $ TestList [test01,test02,test03,test04,test05,test06,test07,test08,test09,test10,test11,test12,test13]