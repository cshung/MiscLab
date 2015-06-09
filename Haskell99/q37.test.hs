import Test.HUnit
import Q37

test01 = TestCase (assertEqual "tryPhi (-1) should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryPhi (-1)))
test02 = TestCase (assertEqual "tryPhi 0    should be Right Nonpositive integer is not supported." (Right "Nonpositive integer is not supported") (tryPhi 0   ))
test03 = TestCase (assertEqual "tryPhi 1    should be Left 1                                    ." (Left 1                                      ) (tryPhi 1   ))
test04 = TestCase (assertEqual "tryPhi 2    should be Left 1                                    ." (Left 1                                      ) (tryPhi 2   ))
test05 = TestCase (assertEqual "tryPhi 3    should be Left 2                                    ." (Left 2                                      ) (tryPhi 3   ))
test06 = TestCase (assertEqual "tryPhi 4    should be Left 2                                    ." (Left 2                                      ) (tryPhi 4   ))
test07 = TestCase (assertEqual "tryPhi 5    should be Left 4                                    ." (Left 4                                      ) (tryPhi 5   ))
test08 = TestCase (assertEqual "tryPhi 6    should be Left 2                                    ." (Left 2                                      ) (tryPhi 6   ))
test09 = TestCase (assertEqual "tryPhi 7    should be Left 6                                    ." (Left 6                                      ) (tryPhi 7   ))
test10 = TestCase (assertEqual "tryPhi 8    should be Left 4                                    ." (Left 4                                      ) (tryPhi 8   ))
test11 = TestCase (assertEqual "tryPhi 9    should be Left 6                                    ." (Left 6                                      ) (tryPhi 9   ))
test12 = TestCase (assertEqual "tryPhi 10   should be Left 4                                    ." (Left 4                                      ) (tryPhi 10  ))

main = runTestTT $ TestList [test01,test02,test03,test04,test05,test06,test07,test08,test09,test10,test11,test12]