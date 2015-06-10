import Test.HUnit
import Q41

test1 = TestCase (assertEqual "tryGoldbachList 2 3 should be Right x <= 2 is not supported." (Right "x <= 2 is not supported") (tryGoldbachList 2 3))
test2 = TestCase (assertEqual "tryGoldbachList 4 3 should be Right x > y is not supported ." (Right "x > y is not supported" ) (tryGoldbachList 4 3))
test3 = TestCase (assertEqual "tryGoldbachList 3 3 should be Left []                      ." (Left []                        ) (tryGoldbachList 3 3))
test4 = TestCase (assertEqual "tryGoldbachList 3 4 should be Left [(2,2)]                 ." (Left [(2,2)]                   ) (tryGoldbachList 3 4))
test5 = TestCase (assertEqual "tryGoldbachList 3 5 should be Left [(2,2)]                 ." (Left [(2,2)]                   ) (tryGoldbachList 3 5))

main = runTestTT $ TestList [test1,test2,test3,test4,test5]