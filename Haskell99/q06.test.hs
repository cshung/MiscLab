import Test.HUnit
import Q06

isPalindromeInt :: [Int] -> Bool
isPalindromeInt = isPalindrome

test1 = TestCase (assertEqual "isPalindrome []    should be True.  " (True)  (isPalindromeInt []   )) 
test2 = TestCase (assertEqual "isPalindrome [1]   should be True.  " (True)  (isPalindrome    [1]  ))
test3 = TestCase (assertEqual "isPalindrome [1,2] should be False. " (False) (isPalindrome    [1,2]))
test4 = TestCase (assertEqual "isPalindrome [1,1] should be True.  " (True)  (isPalindrome    [1,1]))

main = runTestTT $ TestList [test1,test2,test3,test4]