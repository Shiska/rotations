<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" version="5.0" encoding="UTF-8" omit-xml-declaration="yes"/>
<xsl:key name="extra" match="//export/text()" use="."/>
<xsl:template match="/">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title><xsl:value-of select="doc/assembly/name"/></title>
    <style type="text/css">
        #body {
            display: flex;
        }
        #index {
            min-height: 100vh;
        }
        .info, #index {
            width: 100%;
            padding: 0 1em 0 1em;
            border-top: 1px solid;
        }
        .info {
            border-left: 1px solid;
        }
        .hidden {
            display: none;
        }
        .inline {
            display: flex;
            align-items: center;
        }
        .param {
            font-weight: bold;
            font-style: italic;
        }
        .paraminfo {
            font-weight: bold;
            color: #336699;
        }
        code {
            margin: 0;
            padding: 0;
            font-size: small;
        }
        body {
            display: flex;
            font-size: small;
            flex-direction: column;
            font-family: "Verdana", sans-serif;
        }
        h1,h2,h3,h4,th {
            color: #4e4887;
            margin: 0.5em 0.5em 0.5em 0em;
        }
        h1 { font-size: xx-large; }
        h2 { font-size: x-large; }
        h3 { font-size: large; }
        h4 { font-size: medium; }

        table {
            border: medium none;
            background-color: #ddeeff;
        }
        td, td>div>h4 {
            margin: 2px;
            padding: 2px;
            text-align: left;
            font-size: x-small;
            border: medium none;
        }
    </style>
    <script>
        var displayed = []

        function getSection(element) {
            while(element.tagName != "SECTION") {
                element = element.parentElement;
            }
            return element;
        }

        function display(event, id) {
            var dest = document.getElementById(id);

            if(dest) {
                dest = getSection(dest);

                var destidx = displayed.indexOf(dest);
                var src = getSection(event.target);
                var srcidx = displayed.indexOf(src) + 1;

                if(destidx == -1) { // element isn't visible
                    // get position of source section and remove all opened views after it
                    while(displayed.length > srcidx) {
                        displayed.pop().className = "hidden";
                    }
                    dest.className = "info";
                    // move new element behind source
                    src.parentNode.insertBefore(dest, src.nextSibling);
                    // add it to the array
                    displayed.push(dest);
                } else if(destidx > srcidx) { // element is visible but could be further away, so move it down
                    while(displayed.length > srcidx) {
                        displayed.pop().className = "hidden";
                    }
                    dest.className = "info";
                    // no need to move it because it is already behind source
                    displayed.push(dest);
                }
            } else { // id not found
                console.log("id " + id + " not found!");
            }
        }
    </script>
</head>
<body>
    <section>
        <h1><xsl:value-of select="doc/assembly/name"/></h1>
        <xsl:for-each select="doc/general">
            <xsl:call-template name="summary"/>
            <xsl:apply-templates select="*[not(name() = 'summary')]"/>
        </xsl:for-each>
    </section>
    <section id="body">
        <xsl:variable name="members" select="doc/members/member[not(@name='F:__file' or @name='F:__date' or @name='F:__time')]"/>
        <section id="index">
            <xsl:call-template name="indexSection">
                <xsl:with-param name="name" select="'general'"/>
                <xsl:with-param name="export" select="$members[export/. = '']"/>
            </xsl:call-template>
            <xsl:for-each select="$members/export/text()[generate-id() = generate-id(key('extra', .)[1])]">
                <xsl:sort select="."/>
                <xsl:variable name="export" select="."/>

                <xsl:call-template name="indexSection">
                    <xsl:with-param name="name" select="$export"/>
                    <xsl:with-param name="export" select="$members[export/text() = $export]"/>
                </xsl:call-template>
            </xsl:for-each>
            <!-- <xsl:call-template name="indexSection">
                <xsl:with-param name="name" select="'misc'"/>
                <xsl:with-param name="export" select="$members[not(export)]"/>
            </xsl:call-template> -->
        </section>
        <xsl:apply-templates select="$members"/>
    </section>
</body>
</html>
</xsl:template>

<xsl:template match="general">
    <xsl:apply-templates/>
</xsl:template>

<xsl:template match="section"><h2 class="general"><xsl:apply-templates/></h2></xsl:template>
<xsl:template match="subsection"><h3 class="general"><xsl:apply-templates/></h3></xsl:template>

<xsl:template name="indexSection">
    <xsl:param name="name"/>
    <xsl:param name="export"/>

    <xsl:if test="count($export) != 0">
        <xsl:variable name="Name" select="concat(translate(substring($name,1,1),'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'),substring($name,2))"/>
        <h2>
            <xsl:value-of select="$Name"/>
        </h2>
        <ul>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'enumeration'"/>
                <xsl:with-param name="export" select="$export[starts-with(@name,'T:')]"/>
            </xsl:call-template>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'constant'"/>
                <xsl:with-param name="export" select="$export[starts-with(@name,'C:')]"/>
            </xsl:call-template>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'variable'"/>
                <xsl:with-param name="export" select="$export[starts-with(@name,'F:')]"/>
            </xsl:call-template>
            <xsl:call-template name="index">
                <xsl:with-param name="name" select="'function'"/>
                <xsl:with-param name="export" select="$export[starts-with(@name,'M:')]"/>
            </xsl:call-template>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template name="index">
    <xsl:param name="name"/>
    <xsl:param name="export"/>
    <xsl:if test="count($export) != 0">
        <xsl:variable name="Name" select="concat(translate(substring($name,1,1),'ecvf','ECVF'),substring($name,2))"/>
        <h3>
            <xsl:attribute name="class"><xsl:value-of select="$name"/></xsl:attribute>
            <xsl:value-of select="$Name"/>s
        </h3>
        <ul>
            <xsl:for-each select="$export">
                <xsl:variable name="sub" select="substring(@name,3)"/>
                <li>
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="$sub"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template match="member">
    <xsl:variable name="sub" select="substring(@name,3)"/>

    <section>
        <xsl:attribute name="class">hidden</xsl:attribute>
        <xsl:attribute name="id"><xsl:value-of select="$sub"/></xsl:attribute>
        <a>
            <xsl:attribute name="name"><xsl:value-of select="$sub"/></xsl:attribute>
            <h2><xsl:value-of select="$sub"/></h2>
        </a>
        <xsl:call-template name="summary"/>
        <xsl:call-template name="value"/>
        <xsl:call-template name="syntax"/>
        <xsl:apply-templates select="tagname"/>
        <xsl:apply-templates select="size"/>
        <xsl:call-template name="returns"/>
        <xsl:call-template name="remarks"/>
        <xsl:call-template name="enum"/>
        <xsl:call-template name="example"/>
        <xsl:call-template name="referrer"/>
        <xsl:call-template name="dependency"/>
        <xsl:call-template name="attribute"/>
        <xsl:apply-templates select="automaton"/>
        <xsl:call-template name="transition"/>
        <xsl:apply-templates select="stacksize"/>
        <xsl:call-template name="seealso"/>
    </section>
</xsl:template>

<xsl:template name="summary">
    <xsl:if test="summary">
        <p>
            <xsl:for-each select="summary">
                <xsl:apply-templates/><br/>
            </xsl:for-each>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template name="value">
    <xsl:if test="@value">
        <div class="inline">
            <h4>Value</h4>
            <xsl:value-of select="@value"/>
        </div>
    </xsl:if>
</xsl:template>

<xsl:template name="syntax">
    <xsl:if test="@syntax">
        <h4>Syntax</h4>
        <code><xsl:value-of select="@syntax"/></code>
        <xsl:call-template name="param"/>
    </xsl:if>
</xsl:template>

<xsl:key name="param" match="param" use="@name"/>
<xsl:variable name="uniqueParamNames" select="//param[generate-id() = generate-id(key('param', @name)[1])]"/>

<xsl:template name="param">
    <xsl:if test="param">
        <xsl:variable name="param" select="param"/>
        <p>
            <table>
                <xsl:for-each select="$uniqueParamNames"> 
                    <xsl:variable name="name" select="@name"/>
                    <xsl:variable name="data" select="$param[@name = $name]"/>
                    <xsl:if test="count($data) != 0">
                        <tr>
                            <td class="param"><xsl:value-of select="$name"/></td>
                            <td><xsl:apply-templates select="$data"/></td>
                        </tr>
                    </xsl:if>
                </xsl:for-each>
            </table>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template match="paraminfo">
    <xsl:param name="text" select="normalize-space(.)"/>
    <span class="paraminfo">
        &lt;<xsl:choose>
            <xsl:when test="contains($text, ' ')">
                <xsl:call-template name="ref">
                    <xsl:with-param name="id" select="normalize-space(substring-before($text, ' '))"/>
                </xsl:call-template>
                <xsl:value-of select="' '"/>
                <xsl:value-of select="normalize-space(substring-after($text, ' '))"/>
            </xsl:when>
            <xsl:otherwise>
                <xsl:call-template name="ref">
                    <xsl:with-param name="id" select="$text"/>
                </xsl:call-template>
            </xsl:otherwise>
        </xsl:choose>&gt;
    </span>
</xsl:template>

<xsl:template match="tagname">
    <div class="inline">
        <h4>Tag</h4>        
        <xsl:call-template name="ref">
            <xsl:with-param name="id" select="@value"/>
        </xsl:call-template>
    </div>
</xsl:template>

<xsl:template match="size">
    <div class="inline">
        <h4>Size</h4>
        <xsl:value-of select="@value"/>
    </div>
</xsl:template>

<xsl:template name="returns">
    <xsl:if test=".//returns">
        <h4>Returns</h4>
        <p>
            <xsl:choose>
                <xsl:when test=".//returns[@name]">
                    <table>
                        <xsl:for-each select=".//returns">
                            <tr>
                                <td class="param"><xsl:value-of select="@name"/></td>
                                <td><xsl:apply-templates/></td>
                            </tr>
                        </xsl:for-each>
                    </table>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:apply-templates select=".//returns"/>
                </xsl:otherwise>
            </xsl:choose>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template name="remarks">
    <xsl:if test=".//remarks">
        <h4>Remarks</h4>
        <p>
            <xsl:for-each select=".//remarks">
                <xsl:apply-templates/><br/>
            </xsl:for-each>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template name="enum">
    <xsl:if test="member">
        <h4>Members</h4>
        <p>
            <table>
                <xsl:for-each select="member">
                    <xsl:variable name="sub" select="substring(@name,3)"/>
                    <tr>
                        <xsl:attribute name="id"><xsl:value-of select="$sub"/></xsl:attribute>
                        <td class="param">
                            <a>
                                <xsl:attribute name="name"><xsl:value-of select="$sub"/></xsl:attribute>
                                <xsl:value-of select="$sub"/>
                            </a>
                        </td>
                        <td><xsl:call-template name="value"/></td>
                        <td><xsl:apply-templates select="tagname"/></td>
                        <td><xsl:apply-templates select="size"/></td>
                    </tr>
                </xsl:for-each>
            </table>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template name="example">
    <xsl:if test=".//example">
        <h4>Example</h4>
        <table>
            <xsl:for-each select=".//example">
                    <tr><td><code>
                        <xsl:choose>
                            <xsl:when test="text()">
                                <xsl:if test="@tab">
                                    <xsl:attribute name="style">padding-left: <xsl:value-of select="@tab * 2"/>em;</xsl:attribute>
                                </xsl:if>
                                <xsl:apply-templates/>
                            </xsl:when>
                            <xsl:otherwise>
                                <br/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </code></td></tr>
            </xsl:for-each>
    </table>
    </xsl:if>
</xsl:template>

<xsl:template name="referrer">
    <xsl:if test="referrer">
        <h4>Used by</h4>
        <ul>
            <xsl:for-each select="referrer">
                <li class="referrer">
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="@name"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template name="dependency">
    <xsl:if test="dependency">
        <h4>Depends on</h4>
        <ul>
            <xsl:for-each select="dependency">
                <li class="dependency">
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="@name"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template name="attribute">
    <xsl:if test="attribute">
        <h4>Attributes</h4>
        <ul>
            <xsl:for-each select="attribute">
                <li class="attribute"><xsl:value-of select="@name"/></li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template match="automaton">
    <h4>Automaton</h4>
    <p><xsl:value-of select="@name"/></p>
</xsl:template>

<xsl:template name="transition">
    <xsl:if test="transition">
        <h4>Transition table</h4>
        <p>
            <table>
                <tr><th>Source</th><th>Target</th><th>Condition</th></tr>
                <xsl:for-each select="transition">
                    <tr>
                        <td class="transition"><xsl:value-of select="@source"/></td>
                        <td class="transition"><xsl:value-of select="@target"/></td>
                        <td><xsl:value-of select="@condition"/></td>
                    </tr>
                </xsl:for-each>
            </table>
        </p>
    </xsl:if>
</xsl:template>

<xsl:template match="stacksize">
    <h4>Estimated stack usage</h4>
    <ul>
        <li><xsl:value-of select="@value"/> cell<xsl:if test="@value > 1">s</xsl:if></li>
        <li><xsl:value-of select="@value * 4"/> Bytes</li>
    </ul>
</xsl:template>

<xsl:template name="seealso">
    <xsl:if test=".//seealso">
        <h4>See Also</h4>
        <ul>
            <xsl:for-each select=".//seealso">
                <li>
                    <xsl:call-template name="ref">
                        <xsl:with-param name="id" select="@name"/>
                    </xsl:call-template>
                </li>
            </xsl:for-each>
        </ul>
    </xsl:if>
</xsl:template>

<xsl:template match="paramref">
    <i class="param"><xsl:value-of select="@name"/></i>
</xsl:template>

<!-- pawn-lang html tags -->
<xsl:template match="c"><code><xsl:apply-templates/></code></xsl:template>
<xsl:template match="em"><em><xsl:apply-templates/></em></xsl:template>
<xsl:template match="p"><p><xsl:apply-templates/></p></xsl:template>
<xsl:template match="para"><hr class="para"/><xsl:apply-templates/></xsl:template>
<xsl:template match="ul"><ul><xsl:apply-templates/></ul></xsl:template>
<xsl:template match="ol"><ol><xsl:apply-templates/></ol></xsl:template>
<xsl:template match="li"><li><xsl:apply-templates/></li></xsl:template>
<!-- additional html tags -->
<xsl:template match="br"><br/></xsl:template>
<xsl:template match="a">
    <xsl:choose>
        <xsl:when test="node()">
            <a>
                <xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
                <xsl:attribute name="target"><xsl:value-of select="@target"/></xsl:attribute>
                <xsl:apply-templates/>
            </a>
        </xsl:when>
        <xsl:otherwise>
            <a>
                <xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
                <xsl:attribute name="target"><xsl:value-of select="@target"/></xsl:attribute>
                <xsl:value-of select="@href"/>
            </a>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="ref">
    <xsl:call-template name="ref">
        <xsl:with-param name="id" select="@name"/>
    </xsl:call-template>
</xsl:template>

<xsl:template name="ref">
    <xsl:param name="id"/>
    <xsl:if test="$id">
        <xsl:choose>
            <xsl:when test="//member[concat(':', $id) = substring(@name, 2)]">
                <a>
                    <xsl:attribute name="href">#<xsl:value-of select="$id"/></xsl:attribute>
                    <xsl:attribute name="onclick">display(event, '<xsl:value-of select="$id"/>')</xsl:attribute>
                    <xsl:value-of select="$id"/>
                </a>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$id"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:if>
</xsl:template>

</xsl:stylesheet>