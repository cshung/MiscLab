import Test.HUnit
import Q34

test01 = TestCase (assertEqual "tryTotient (-1) should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryTotient (-1)))
test02 = TestCase (assertEqual "tryTotient 0    should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryTotient 0   ))
test03 = TestCase (assertEqual "tryTotient 1    should be Left 1                                    ." (Left 1                                      ) (tryTotient 1   ))
test04 = TestCase (assertEqual "tryTotient 2    should be Left 1                                    ." (Left 1                                      ) (tryTotient 2   ))
test05 = TestCase (assertEqual "tryTotient 3    should be Left 2                                    ." (Left 2                                      ) (tryTotient 3   ))
test06 = TestCase (assertEqual "tryTotient 4    should be Left 2                                    ." (Left 2                                      ) (tryTotient 4   ))
test07 = TestCase (assertEqual "tryTotient 5    should be Left 4                                    ." (Left 4                                      ) (tryTotient 5   ))
test08 = TestCase (assertEqual "tryTotient 6    should be Left 2                                    ." (Left 2                                      ) (tryTotient 6   ))
test09 = TestCase (assertEqual "tryTotient 7    should be Left 6                                    ." (Left 6                                      ) (tryTotient 7   ))
test10 = TestCase (assertEqual "tryTotient 8    should be Left 4                                    ." (Left 4                                      ) (tryTotient 8   ))
test11 = TestCase (assertEqual "tryTotient 9    should be Left 6                                    ." (Left 6                                      ) (tryTotient 9   ))
test12 = TestCase (assertEqual "tryTotient 10   should be Left 4                                    ." (Left 4                                      ) (tryTotient 10  ))

main = runTestTT $ TestList [test01,test02,test03,test04,test05,test06,test07,test08,test09,test10,test11,test12]