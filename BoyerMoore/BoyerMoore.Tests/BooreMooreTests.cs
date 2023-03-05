using Xunit;
using System.Linq;

namespace BoyerMoore.Tests
{
    public class BooreMooreTests
    {
        [Fact]
        public void HelloWorld()
        {
            Assert.Equal(new int[] { 6 }, BoyerMoore.IndexesOf("Hello World", "World").ToArray());
        }

        [Fact]
        public void CruelWorld()
        {
            Assert.Equal(new int[] { }, BoyerMoore.IndexesOf("Hello World", "Cruel").ToArray());
        }

        [Fact]
        public void MatchSuffixAndShift()
        {
            Assert.Equal(new int[] { 2 }, BoyerMoore.IndexesOf("pppqpq", "pqpq").ToArray());
        }

        [Fact]
        public void MatchOverlapPattern()
        {
            Assert.Equal(new int[] { 0, 2 }, BoyerMoore.IndexesOf("pqpqpq", "pqpq").ToArray());
        }

        [Fact]
        public void MultipleBorders()
        {
            Assert.Equal(new int[] { 0, 2, 4 }, BoyerMoore.IndexesOf("ababababa", "ababa").ToArray());
        }

        // Special thanks to Pieter Witvoet for contributing these tests.
        // https://codereview.stackexchange.com/questions/210514/leetcode-28-boyer-moore-string-matching-algorithms

        [Fact]
        public void Regression1()
        {
            //                                                  012345
            //                                                       BC..ABC
            Assert.Equal(new int[] { 5 }, BoyerMoore.IndexesOf("BC.xABC..ABC", "BC..ABC").ToArray());
        }

        [Fact]
        public void Regression2()
        {
            //                                                  012345
            //                                                       ..ABC
            Assert.Equal(new int[] { 5 }, BoyerMoore.IndexesOf("x.ABC..ABC", "..ABC").ToArray());
        }

        [Fact]
        public void Regression3()
        {
            Assert.Equal(new int[] { }, BoyerMoore.IndexesOf("bbababaaaababbaabbbabbbaaabbbaaababbabaabbaaaaabbaaabbbbaaabaabbaababbbaabaaababbaaabbbbbbaabbbbbaaabbababaaaaabaabbbababbaababaabbaa", "bbabba").ToArray());
        }

        [Fact]
        public void Regression4()
        {
            Assert.Equal(new int[] { 0 }, BoyerMoore.IndexesOf("a", "a").ToArray());
        }
    }
}
