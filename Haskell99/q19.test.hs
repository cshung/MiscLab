import Test.HUnit
import Q19

test1 = TestCase (assertEqual "rotate [1,2,3] (-3) should be [1,2,3]." ([1,2,3]) (rotate [1,2,3] (-3)))
test2 = TestCase (assertEqual "rotate [1,2,3] (-2) should be [2,3,1]." ([2,3,1]) (rotate [1,2,3] (-2)))
test3 = TestCase (assertEqual "rotate [1,2,3] (-1) should be [3,1,2]." ([3,1,2]) (rotate [1,2,3] (-1)))
test4 = TestCase (assertEqual "rotate [1,2,3] 0    should be [1,2,3]." ([1,2,3]) (rotate [1,2,3] 0   ))
test5 = TestCase (assertEqual "rotate [1,2,3] 1    should be [2,3,1]." ([2,3,1]) (rotate [1,2,3] 1   ))
test6 = TestCase (assertEqual "rotate [1,2,3] 2    should be [3,1,2]." ([3,1,2]) (rotate [1,2,3] 2   ))
test7 = TestCase (assertEqual "rotate [1,2,3] 3    should be [1,2,3]." ([1,2,3]) (rotate [1,2,3] 3   ))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6,test7]