import Test.HUnit
import Q32

test1 = TestCase (assertEqual "tryMyGCD 0     15    should be Right Zero is not supported. " (Right "Zero is not supported") (tryMyGCD 0     15   ))
test2 = TestCase (assertEqual "tryMyGCD 15    0     should be Right Zero is not supported. " (Right "Zero is not supported") (tryMyGCD 15    0    ))
test3 = TestCase (assertEqual "tryMyGCD 12    32    should be Left 4                     . " (Left 4                       ) (tryMyGCD 12    32   ))
test4 = TestCase (assertEqual "tryMyGCD 32    12    should be Left 4                     . " (Left 4                       ) (tryMyGCD 32    12   ))
test5 = TestCase (assertEqual "tryMyGCD (-32) 12    should be Left 4                     . " (Left 4                       ) (tryMyGCD (-32) 12   ))
test6 = TestCase (assertEqual "tryMyGCD 32    (-12) should be Left 4                     . " (Left 4                       ) (tryMyGCD 32    (-12)))
test7 = TestCase (assertEqual "tryMyGCD (-12) 32    should be Left 4                     . " (Left 4                       ) (tryMyGCD (-12) 32   ))
test8 = TestCase (assertEqual "tryMyGCD 12    (-32) should be Left 4                     . " (Left 4                       ) (tryMyGCD 12    (-32)))

main = runTestTT $ TestList [test1,test2,test3,test4,test5,test6,test7,test8]