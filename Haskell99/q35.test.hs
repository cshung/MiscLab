import Test.HUnit
import Q35

test01 = TestCase (assertEqual "tryPrimeFactors (-1) should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryPrimeFactors (-1)))
test02 = TestCase (assertEqual "tryPrimeFactors 0    should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryPrimeFactors 0   ))
test03 = TestCase (assertEqual "tryPrimeFactors 1    should be Left []                                   ." (Left []                                     ) (tryPrimeFactors 1   ))
test04 = TestCase (assertEqual "tryPrimeFactors 2    should be Left [2]                                  ." (Left [2]                                    ) (tryPrimeFactors 2   ))
test05 = TestCase (assertEqual "tryPrimeFactors 3    should be Left [3]                                  ." (Left [3]                                    ) (tryPrimeFactors 3   ))
test06 = TestCase (assertEqual "tryPrimeFactors 4    should be Left [2,2]                                ." (Left [2,2]                                  ) (tryPrimeFactors 4   ))
test07 = TestCase (assertEqual "tryPrimeFactors 5    should be Left [5]                                  ." (Left [5]                                    ) (tryPrimeFactors 5   ))
test08 = TestCase (assertEqual "tryPrimeFactors 6    should be Left [2,3]                                ." (Left [2,3]                                  ) (tryPrimeFactors 6   ))
test09 = TestCase (assertEqual "tryPrimeFactors 7    should be Left [7]                                  ." (Left [7]                                    ) (tryPrimeFactors 7   ))
test10 = TestCase (assertEqual "tryPrimeFactors 8    should be Left [2,2,2]                              ." (Left [2,2,2]                                ) (tryPrimeFactors 8   ))
test11 = TestCase (assertEqual "tryPrimeFactors 9    should be Left [3,3]                                ." (Left [3,3]                                  ) (tryPrimeFactors 9   ))
test12 = TestCase (assertEqual "tryPrimeFactors 10   should be Left [2,5]                                ." (Left [2,5]                                  ) (tryPrimeFactors 10  ))

main = runTestTT $ TestList [test01,test02,test03,test04,test05,test06,test07,test08,test09,test10,test11,test12]