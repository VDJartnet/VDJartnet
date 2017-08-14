<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>

  <xsl:template match="choices-outline|choice|pkg-ref|options"/>

  <xsl:template match="installer-gui-script">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
      <options customize="always" require-scripts="true" allow-external-scripts="true"/>
      <title>VDJartnet</title>
      <license file="COPYING"/>
      <readme file="README.html" mime-type="text/html" uti="public.html"/>
      <domains enable_localSystem="false" enable_anywhere="false" enable_currentUserHome="true"/>
      <choices-outline>
        <line choice="uk.kepler22b.vdjartnet.vdjartnet"/>
        <line choice="uk.kepler22b.vdjartnet.files">
          <line choice="uk.kepler22b.vdjartnet.config"/>
          <line choice="uk.kepler22b.vdjartnet.presets"/>
        </line>
      </choices-outline>
      <choice id="uk.kepler22b.vdjartnet.vdjartnet" description="Install VDJartnet" title="VDJartnet" enabled="false" visible="true">
        <pkg-ref id="uk.kepler22b.vdjartnet.vdjartnet"/>
      </choice>
      <pkg-ref id="uk.kepler22b.vdjartnet.vdjartnet" version="0" onConclusion="none">component-vdjartnet.pkg</pkg-ref>
      <choice id="uk.kepler22b.vdjartnet.files" description="Install updated config and preset files" title="Files" enabled="false" visible="true"/>
      <choice id="uk.kepler22b.vdjartnet.config" description="Install updated config.txt" title="config.txt" visible="true" start_selected="true" enabled="system.run(&quot;if [[ -e \&quot;~/Documents/VirtualDJ/Plugins64/AutoStart/VDJartnet/config.txt\&quot; ]]; then exit 0; else exit 1; fi&quot;)">
        <pkg-ref id="uk.kepler22b.vdjartnet.config"/>
      </choice>
      <pkg-ref id="uk.kepler22b.vdjartnet.config" version="0" onConclusion="none">component-config.pkg</pkg-ref>
      <choice id="uk.kepler22b.vdjartnet.presets" description="Install updated presets.txt" title="presets.txt" visible="true" start_selected="true" enabled="system.run(&quot;if [[ -e \&quot;~/Documents/VirtualDJ/Plugins64/AutoStart/VDJartnet/presets.txt\&quot; ]]; then exit 0; else exit 1; fi&quot;)">
        <pkg-ref id="uk.kepler22b.vdjartnet.presets"/>
      </choice>
      <pkg-ref id="uk.kepler22b.vdjartnet.presets" version="0" onConclusion="none">component-presets.pkg</pkg-ref>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
