namespace SuffixTreePrototype
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Text;

    // Still O(m^3) - building suffix links
    class SuffixTree
    {
        private static bool assertOn = true;

        private SuffixTree(string text)
        {
            // Avoid direct construction
            this.text = text;
        }

        public static SuffixTree Build(string text)
        {
            SuffixTree result = new SuffixTree(text);
            result.BuildTree(text);
            return result;
        }

        private void BuildTree(string text)
        {
            // Step 1: Build initial tree
            this.rootNode = new SuffixTreeNode(0, 1);
            this.fullStringLeaf = new SuffixTreeNode(1, 1);
            SuffixTreeLink nodeToFullStringLeafLink = new SuffixTreeLink { Parent = rootNode, Start = 0, End = 1, Child = fullStringLeaf };
            this.rootNode.links.Add(text[0], nodeToFullStringLeafLink);
            fullStringLeaf.ParentLink = nodeToFullStringLeafLink;

            // debugging
            this.extensionCount = 0;

            // Step 2: Basic extension loop
            for (int endIndex = 2; endIndex <= text.Length; endIndex++)
            {
                this.lastExtensionSearchNode = null;
                for (int startIndex = 0; startIndex < endIndex; startIndex++)
                {
                    if (this.Extend(startIndex, endIndex))
                    {
                        // Short circuit to the next phase if rule 3 ever applied
                        break;
                    }
                }
            }

            if (assertOn)
            {
                ValidateSuffixLinks(this.rootNode);
            }
        }

        // startIndex, endIndex are 0 based [) index to the string to be inserted to the current implicit suffix tree
        private bool Extend(int startIndex, int endIndex)
        {
            this.extensionCount++;

            int insertingTextLength = endIndex - startIndex;
            int searchingTextLength = insertingTextLength - 1;
            //
            // Step 1: Search for the end of the searchingText
            //
            // The search result could be 
            // nodeCursor == rootNode, in this case the searchingText is the empty string
            // followingLink != null and 0 < linkCursor <= followingLink.Length(), that represents the exclusive end index of a pointer on the link
            // note that if linkCursor == followingLink.Length(), it means we are at the followingLink.child node
            //
            // We decided to use followingLink != null && linkCursor == followingLink.Length() to represent the cursor is at the next node is because we need 
            // to manipulate the link at times (e.g. extending it in leaf case)
            // 
            SuffixTreeNode nodeCursor;
            SuffixTreeLink followingLink;
            int linkCursor;
            this.SearchPrefix(startIndex, searchingTextLength, out nodeCursor, out followingLink, out linkCursor);

            //
            // Step 2: Extend the tree by the last character
            //
            return ExtendByLastCharacter(insertingTextLength, endIndex, nodeCursor, followingLink, linkCursor);
        }

        private void SearchPrefix(int startIndex, int searchingTextLength, out SuffixTreeNode nodeCursor, out SuffixTreeLink followingLink, out int linkCursor)
        {
            nodeCursor = this.rootNode;
            followingLink = null;
            linkCursor = -1;

            if (startIndex == 0)
            {
                // For the first extension - it is always extending the full string leaf node
                followingLink = this.fullStringLeaf.ParentLink;
                linkCursor = followingLink.Length();
            }
            else
            {
                SuffixTreeNode startNode = this.rootNode;
                Debug.Assert(this.lastExtensionSearchNode != null, "Last extension should have searched something");
                if (this.lastExtensionSearchNode != this.rootNode)
                {
                    Debug.Assert(this.lastExtensionSearchNode.ParentLink != null, "A node should have a parent link if it is not a root node");
                    Debug.Assert(this.lastExtensionSearchNode.ParentLink.Parent != null, "A link should have a parent node");
                    if (this.lastExtensionSearchNode.SuffixLink == null)
                    {
                        if (this.lastExtensionSearchNode.ParentLink.Parent != this.rootNode)
                        {
                            Debug.Assert(this.lastExtensionSearchNode.ParentLink.Parent.SuffixLink != null);
                            startNode = this.lastExtensionSearchNode.ParentLink.Parent.SuffixLink;
                        }
                    }
                    else
                    {
                        startNode = this.lastExtensionSearchNode.SuffixLink;
                    }
                }

                int remainingSearchingTextLength = searchingTextLength;
                int textCursor = startIndex;

                // Short-circuitting by suffix link
                remainingSearchingTextLength -= startNode.StringDepth;
                textCursor += startNode.StringDepth;
                nodeCursor = startNode;

                while (remainingSearchingTextLength > 0)
                {
                    char currentCharacter = text[textCursor];
                    if (followingLink == null)
                    {
                        followingLink = nodeCursor.links[currentCharacter];
                        nodeCursor = null;
                        linkCursor = 0;
                    }
                    else
                    {
                        if (linkCursor == followingLink.Length())
                        {
                            linkCursor = -1;
                            nodeCursor = followingLink.Child;
                            followingLink = null;
                        }
                        else
                        {
                            int move = Math.Min(followingLink.Length(), remainingSearchingTextLength);
                            if (assertOn)
                            {
                                string movedText = text.Substring(textCursor, move);
                                string movedLink = text.Substring(followingLink.Start, move);
                                Debug.Assert(string.Equals(movedText, movedLink));
                            }

                            textCursor += move;
                            remainingSearchingTextLength -= move;
                            linkCursor += move;
                        }
                    }
                }
            }
        }

        private bool ExtendByLastCharacter(int insertingTextLength, int endIndex, SuffixTreeNode nodeCursor, SuffixTreeLink followingLink, int linkCursor)
        {
            bool rule3Applied = false;
            char characterToExtend = text[endIndex - 1];
            SuffixTreeNode newlyCreatedInternalNode = null;
            if (followingLink != null)
            {
                if (linkCursor == followingLink.Length())
                {
                    if (followingLink.Child.links.Count == 0)
                    {
                        // Rule 1 - we have reached a leaf - extend the leaf, done
                        followingLink.End++;
                        followingLink.Child.StringDepth++;
                        this.lastExtensionSearchNode = followingLink.Child.ParentLink.Parent;
                    }
                    else
                    {
                        if (followingLink.Child.links.ContainsKey(characterToExtend))
                        {
                            if (assertOn)
                            {
                                Debug.Assert(this.lastInternalNode == null, "Why do we have a last internal node while we end up in rule 3?");
                            }

                            rule3Applied = true;
                        }
                        else
                        {
                            // Rule 2 - but not creating internal nodes
                            SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength, 1);
                            SuffixTreeLink newExtendingLink = new SuffixTreeLink { Parent = followingLink.Child, Start = endIndex - 1, End = endIndex, Child = newLeaf };
                            newLeaf.ParentLink = newExtendingLink;
                            followingLink.Child.links.Add(characterToExtend, newExtendingLink);
                            this.lastExtensionSearchNode = followingLink.Child;
                        }
                    }
                }
                else
                {
                    if (followingLink.EdgeLabel(text, linkCursor) == characterToExtend)
                    {
                        if (assertOn)
                        {
                            Debug.Assert(this.lastInternalNode == null, "Why do we have a last internal node while we end up in rule 3?");
                        }

                        rule3Applied = true;
                    }
                    else
                    {
                        // Rule 2 - creating internal node
                        int originalEnd = followingLink.End;
                        SuffixTreeNode originalChild = followingLink.Child;
                        newlyCreatedInternalNode = followingLink.Child = new SuffixTreeNode(insertingTextLength - 1, 1);
                        newlyCreatedInternalNode.ParentLink = followingLink;
                        followingLink.End = followingLink.Start + linkCursor;
                        SuffixTreeLink brokenLink2 = new SuffixTreeLink { Parent = followingLink.Child, Start = followingLink.Start + linkCursor, End = originalEnd, Child = originalChild };
                        originalChild.ParentLink = brokenLink2;
                        followingLink.Child.links.Add(text[followingLink.Start + linkCursor], brokenLink2);

                        SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength, 1);
                        SuffixTreeLink newExtendingLink = new SuffixTreeLink { Parent = followingLink.Child, Start = endIndex - 1, End = endIndex, Child = newLeaf };
                        newLeaf.ParentLink = newExtendingLink;
                        followingLink.Child.links.Add(characterToExtend, newExtendingLink);
                        this.lastExtensionSearchNode = newlyCreatedInternalNode;
                    }
                }
            }
            else
            {
                if (nodeCursor.links.ContainsKey(characterToExtend))
                {
                    rule3Applied = true;
                }
                else
                {
                    // Rule 2 - but not creating internal nodes
                    SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength, 1);
                    SuffixTreeLink newExtendingLink = new SuffixTreeLink { Parent = nodeCursor, Start = endIndex - 1, End = endIndex, Child = newLeaf };
                    newLeaf.ParentLink = newExtendingLink;
                    nodeCursor.links.Add(characterToExtend, newExtendingLink);
                    this.lastExtensionSearchNode = nodeCursor;
                }
            }

            if (this.lastInternalNode != null)
            {
                if (assertOn)
                {
                    Debug.Assert(lastInternalNodeBirthday == this.extensionCount - 1);
                }
                if (followingLink != null)
                {
                    if (assertOn)
                    {
                        Debug.Assert(linkCursor == followingLink.Length(), "We should always end up in another node if we applied rule 2");
                    }
                    this.lastInternalNode.SuffixLink = followingLink.Child;
                }
                else
                {
                    this.lastInternalNode.SuffixLink = nodeCursor;
                }

                this.lastInternalNode = null;
            }

            if (newlyCreatedInternalNode != null)
            {
                this.lastInternalNode = newlyCreatedInternalNode;
                this.lastInternalNodeBirthday = this.extensionCount;
            }

            return rule3Applied;
        }

        // debugging
        private int extensionCount;
        private int lastInternalNodeBirthday;

        private void ValidateSuffixLinks(SuffixTreeNode node)
        {
            if (node.ParentLink == null)
            {
                Debug.Assert(node.SuffixLink == null, "Root node should not have a suffix link");
            }
            else if (node.links.Count == 0)
            {
                Debug.Assert(node.SuffixLink == null, "Leaf node should not have a suffix link");
            }
            else
            {
                Debug.Assert(node.SuffixLink != null, "Internal node should have suffix link");
                Debug.Assert(node.StringDepth == node.SuffixLink.StringDepth + 1, "Should actually point to a suffix");
            }

            foreach (var link in node.links)
            {
                ValidateSuffixLinks(link.Value.Child);
            }
        }

        private class SuffixTreeLink
        {
            // 0 based [) index
            public char EdgeLabel(string text, int index)
            {
                return text[this.Start + index];
            }

            public string EdgeLabel(string text)
            {
                return text.Substring(this.Start, this.Length());
            }

            public int Length()
            {
                return this.End - this.Start;
            }

            public SuffixTreeNode Parent { get; set; }

            public int Start { get; set; }

            public int End { get; set; }

            public SuffixTreeNode Child { get; set; }
        }

        private class SuffixTreeNode
        {
            public SuffixTreeNode(int stringDepth, int fuckedAgain)
            {
                this.StringDepth = stringDepth;
            }

            public Dictionary<char, SuffixTreeLink> links = new Dictionary<char, SuffixTreeLink>();

            public SuffixTreeLink ParentLink { get; set; }

            public SuffixTreeNode SuffixLink { get; set; }

            public int StringDepth { get; set; }

            internal void BuildString(string text, StringBuilder sb, int indent)
            {
                sb.Append(new string('+', indent));
                sb.AppendFormat("({0})", this.StringDepth);
                sb.AppendLine();

                foreach (var link in links)
                {
                    sb.Append(new string('-', indent));
                    sb.AppendLine(link.Value.EdgeLabel(text));
                    link.Value.Child.BuildString(text, sb, indent + 2);
                }
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            rootNode.BuildString(this.text, sb, 0);
            return sb.ToString();
        }

        private SuffixTreeNode rootNode;
        private SuffixTreeNode fullStringLeaf;
        private SuffixTreeNode lastInternalNode;
        private SuffixTreeNode lastExtensionSearchNode;
        private string text;
    }

    class Program
    {
        static void Main(string[] args)
        {
            SuffixTree result = SuffixTree.Build("xabxac");
            Console.WriteLine(result);
        }
    }
}
