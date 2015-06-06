import Test.HUnit
import Q23

test1 = TestCase (assertEqual "rndSelectTest [1,2,3] 0 should be Right Selecting less than one element is not supported    ." (Right "Selecting less than one element is not supported"    ) (rndSelectTest [1,2,3] 0))
test2 = TestCase (assertEqual "rndSelectTest [1,2,3] 1 should be Left [1]                                                  ." (Left [1]                                                    ) (rndSelectTest [1,2,3] 1))
test3 = TestCase (assertEqual "rndSelectTest [1,2,3] 2 should be Left [1,3]                                                ." (Left [1,3]                                                  ) (rndSelectTest [1,2,3] 2))
test4 = TestCase (assertEqual "rndSelectTest [1,2,3] 3 should be Left [1,3,2]                                              ." (Left [1,3,2]                                                ) (rndSelectTest [1,2,3] 3))
test5 = TestCase (assertEqual "rndSelectTest [1,2,3] 4 should be Right Selecting more than length elements is not supported." (Right "Selecting more than length elements is not supported") (rndSelectTest [1,2,3] 4))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]