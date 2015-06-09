import Test.HUnit
import Q36

test01 = TestCase (assertEqual "tryPrimeFactorsMult (-1) should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryPrimeFactorsMult (-1)))
test02 = TestCase (assertEqual "tryPrimeFactorsMult 0    should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryPrimeFactorsMult 0   ))
test03 = TestCase (assertEqual "tryPrimeFactorsMult 1    should be Left []                                   ." (Left []                                   ) (tryPrimeFactorsMult 1   ))
test04 = TestCase (assertEqual "tryPrimeFactorsMult 2    should be Left [(2,1)]                              ." (Left [(2,1)]                              ) (tryPrimeFactorsMult 2   ))
test05 = TestCase (assertEqual "tryPrimeFactorsMult 3    should be Left [(3,1)]                              ." (Left [(3,1)]                              ) (tryPrimeFactorsMult 3   ))
test06 = TestCase (assertEqual "tryPrimeFactorsMult 4    should be Left [(2,2)]                              ." (Left [(2,2)]                              ) (tryPrimeFactorsMult 4   ))
test07 = TestCase (assertEqual "tryPrimeFactorsMult 5    should be Left [(5,1)]                              ." (Left [(5,1)]                              ) (tryPrimeFactorsMult 5   ))
test08 = TestCase (assertEqual "tryPrimeFactorsMult 6    should be Left [(2,1),(3,1)]                        ." (Left [(2,1),(3,1)]                        ) (tryPrimeFactorsMult 6   ))
test09 = TestCase (assertEqual "tryPrimeFactorsMult 7    should be Left [(7,1)]                              ." (Left [(7,1)]                              ) (tryPrimeFactorsMult 7   ))
test10 = TestCase (assertEqual "tryPrimeFactorsMult 8    should be Left [(2,3)]                              ." (Left [(2,3)]                              ) (tryPrimeFactorsMult 8   ))
test11 = TestCase (assertEqual "tryPrimeFactorsMult 9    should be Left [(3,2)]                              ." (Left [(3,2)]                              ) (tryPrimeFactorsMult 9   ))
test12 = TestCase (assertEqual "tryPrimeFactorsMult 10   should be Left [(2,1),(5,1)]                        ." (Left [(2,1),(5,1)]                        ) (tryPrimeFactorsMult 10  ))

main = runTestTT $ TestList [test01,test02,test03,test04,test05,test06,test07,test08,test09,test10,test11,test12]