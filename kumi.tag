<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="struct">
    <name>kwk::any_allocator</name>
    <filename>structkwk_1_1any__allocator.html</filename>
    <member kind="function">
      <type></type>
      <name>any_allocator</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>a4f1fe0fde7492cce972cbd55c4e28b45</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>any_allocator</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>a378d900d65f57091b898fdc1dae76229</anchor>
      <arglist>(any_allocator &amp;&amp;a)=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>any_allocator</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>a305efbb34f01df73fe43d5196869ee1f</anchor>
      <arglist>(any_allocator const &amp;a)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>any_allocator</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>a69f84bfabf22762e288a392855323a3c</anchor>
      <arglist>(T &amp;&amp;other)</arglist>
    </member>
    <member kind="function">
      <type>api_t *</type>
      <name>get</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>aabe3eff0e718e783880a4eaac4d42fac</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>any_allocator &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>aca119d31801c2883ade0db948f9286eb</anchor>
      <arglist>(any_allocator &amp;&amp;other)=default</arglist>
    </member>
    <member kind="function">
      <type>any_allocator &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>aac44b83ad0777de3cc48f313dde1cc6f</anchor>
      <arglist>(any_allocator const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>any_allocator &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>adde9139e12a6595215c8d28aa25fca4c</anchor>
      <arglist>(T &amp;&amp;other)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>swap</name>
      <anchorfile>structkwk_1_1any__allocator.html</anchorfile>
      <anchor>a0cbb742f7c6c21b8a2800990f35a6df1</anchor>
      <arglist>(any_allocator &amp;other) noexcept</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>kwk::heap_allocator</name>
    <filename>structkwk_1_1heap__allocator.html</filename>
  </compound>
  <compound kind="struct">
    <name>kwk::is_dynamic_extent</name>
    <filename>structkwk_1_1is__dynamic__extent.html</filename>
    <templarg>std::int32_t N</templarg>
    <templarg>auto Desc</templarg>
  </compound>
  <compound kind="struct">
    <name>kwk::is_joker</name>
    <filename>structkwk_1_1is__joker.html</filename>
    <templarg>typename T</templarg>
  </compound>
  <compound kind="struct">
    <name>kwk::is_static_extent</name>
    <filename>structkwk_1_1is__static__extent.html</filename>
    <templarg>std::int32_t N</templarg>
    <templarg>auto Desc</templarg>
  </compound>
  <compound kind="struct">
    <name>kwk::joker</name>
    <filename>structkwk_1_1joker.html</filename>
  </compound>
  <compound kind="struct">
    <name>kwk::shape</name>
    <filename>structkwk_1_1shape.html</filename>
    <templarg>auto... D</templarg>
    <member kind="function">
      <type>constexpr</type>
      <name>shape</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a38dddee955c71802f309259c90016a32</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>shape</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a556523cc0f32af59686754745c0a3e4d</anchor>
      <arglist>(shape const &amp;d) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>shape</name>
      <anchorfile>structkwk_1_1shape_aeba960aa4cd8a10d78d08b902bab9f98.html</anchorfile>
      <anchor>aeba960aa4cd8a10d78d08b902bab9f98</anchor>
      <arglist>(T... d) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>contains</name>
      <anchorfile>structkwk_1_1shape_aafa5833178de42072149b2d631d8c87c.html</anchorfile>
      <anchor>aafa5833178de42072149b2d631d8c87c</anchor>
      <arglist>(Coords... p) const noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>explicit</name>
      <anchorfile>structkwk_1_1shape_a521109d8051702c60bb4a0c2b8bff23a.html</anchorfile>
      <anchor>a521109d8051702c60bb4a0c2b8bff23a</anchor>
      <arglist>(static_order !=sizeof...(D2)) const expr shape(shape&lt; D2... &gt; const &amp;other) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>fit_constraints</name>
      <anchorfile>structkwk_1_1shape_a9d88738ece8673d9d8d6339f07b92ac5.html</anchorfile>
      <anchor>a9d88738ece8673d9d8d6339f07b92ac5</anchor>
      <arglist>(shape&lt; D2... &gt; const &amp;ref) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr std::int32_t</type>
      <name>nbdims</name>
      <anchorfile>structkwk_1_1shape_a2870724c1239b0ab586934f4eab0cc09.html</anchorfile>
      <anchor>a2870724c1239b0ab586934f4eab0cc09</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr std::int32_t</type>
      <name>numel</name>
      <anchorfile>structkwk_1_1shape_a3007ac267d3dac8441e691340bca2a8e.html</anchorfile>
      <anchor>a3007ac267d3dac8441e691340bca2a8e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>operator()</name>
      <anchorfile>structkwk_1_1shape_acb9e3582270f53c087a2ebbd8cf5f288.html</anchorfile>
      <anchor>acb9e3582270f53c087a2ebbd8cf5f288</anchor>
      <arglist>(Slicers const &amp;... s) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr shape &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a768d805dde8ff07081a2f13a3da1a0fa</anchor>
      <arglist>(shape const &amp;other) &amp;noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static constexpr auto</type>
      <name>order</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a26b11c0594a9e0a25431f05c5709b272</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>is_dynamic</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a93aa2f49f015ddc42682e7bbe4178af4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>is_fully_dynamic</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>ad7cc4178635b97a47f771e630d97da41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>is_fully_static</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a14d93ff03c503872967f01da532a594f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr std::int32_t</type>
      <name>static_order</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>aef516befd154a7361b535d04bd2759fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>a088bde0950176b1fc6985e4d222002ba</anchor>
      <arglist>(std::ostream &amp;os, shape const &amp;s)</arglist>
    </member>
    <member kind="friend">
      <type>friend constexpr bool</type>
      <name>operator==</name>
      <anchorfile>structkwk_1_1shape.html</anchorfile>
      <anchor>af5ebfc332017e2cf71f06a97f8b0cb80</anchor>
      <arglist>(shape const &amp;a, shape&lt; D2... &gt; const &amp;b) noexcept</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>kwk::stride</name>
    <filename>structkwk_1_1stride.html</filename>
    <templarg>auto... Strides</templarg>
    <member kind="function">
      <type>constexpr</type>
      <name>stride</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>a77b4894de2c5d1daf4353fa25dbc741e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>stride</name>
      <anchorfile>structkwk_1_1stride_a193106ff05c7f71d4cb6cabebe876279.html</anchorfile>
      <anchor>a193106ff05c7f71d4cb6cabebe876279</anchor>
      <arglist>(concepts::extent auto... values) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>linearize</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>ae51cf0a1e1fd47613929afe91753bc98</anchor>
      <arglist>(Is... is) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>operator==</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>a1b20d33c689da3d4eb8b9cbfc6f8afd9</anchor>
      <arglist>(stride&lt; S2... &gt; const &amp;other) const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static constexpr auto</type>
      <name>order</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>a47b7f8c590ba4e5faf78b8006f768b71</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr auto</type>
      <name>is_unit</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>a93e186840a9ab6e2d64a4f438e74882d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr std::int32_t</type>
      <name>static_order</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>a2254b1dd7442fcc843442b2a5ce0fc1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>structkwk_1_1stride.html</anchorfile>
      <anchor>a5e5407003212107f0f9346c4d75520c3</anchor>
      <arglist>(std::ostream &amp;os, stride const &amp;s)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>kwk::table</name>
    <filename>structkwk_1_1table.html</filename>
    <templarg>typename Builder</templarg>
    <member kind="typedef">
      <type>typename parent::const_pointer</type>
      <name>const_pointer</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ab721a99e0e25695b48949e348836e288</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::const_reference</type>
      <name>const_reference</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a32bb28521398da754d4ae29962c8f80a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::pointer</type>
      <name>pointer</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>aa9affd51108a782b218a01439ed8872c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::reference</type>
      <name>reference</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ae6791b5f53e0f80bf3bb5afac70600e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::shape_type</type>
      <name>shape_type</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ad15f56bb61637a7cb51a727eddc71ac3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::value_type</type>
      <name>value_type</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a5d510776e9e21567496f525718c2c870</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>adb9ef9d36a6392ffcc1d28c66b521b40</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a51452ca716dc0e8387cfd8b34ff745d4</anchor>
      <arglist>(rbr::concepts::option auto const &amp;... opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ac7376cc2405b06cfcb2768791dc2c2bf</anchor>
      <arglist>(rbr::concepts::settings auto const &amp;opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ac8f59477f1c2b6ee29e0b6d98e2151fa</anchor>
      <arglist>(table &amp;&amp;)=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr table &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a112d2e2a816cab550b0463c0d02459d3</anchor>
      <arglist>(table &amp;&amp;)=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a33f62be9622baf7cca1a95488b798ff9</anchor>
      <arglist>(table const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>af938bc11268bfed364ecc893ebabddad</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr table &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a2b6d2f842750d336afd11e6348929772</anchor>
      <arglist>(table const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr table &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a15dc8264a94e20fe389375058b9a81ee</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr std::int32_t</type>
      <name>static_order</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>af9a6a3bcaa5117b7b0af37ef6e4ae09e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>adb9ef9d36a6392ffcc1d28c66b521b40</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a51452ca716dc0e8387cfd8b34ff745d4</anchor>
      <arglist>(rbr::concepts::option auto const &amp;... opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ac7376cc2405b06cfcb2768791dc2c2bf</anchor>
      <arglist>(rbr::concepts::settings auto const &amp;opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>ac8f59477f1c2b6ee29e0b6d98e2151fa</anchor>
      <arglist>(table &amp;&amp;)=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr table &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a112d2e2a816cab550b0463c0d02459d3</anchor>
      <arglist>(table &amp;&amp;)=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a33f62be9622baf7cca1a95488b798ff9</anchor>
      <arglist>(table const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>table</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>af938bc11268bfed364ecc893ebabddad</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr table &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a2b6d2f842750d336afd11e6348929772</anchor>
      <arglist>(table const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr table &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1table.html</anchorfile>
      <anchor>a15dc8264a94e20fe389375058b9a81ee</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>kwk::unsupported_call</name>
    <filename>structkwk_1_1unsupported__call.html</filename>
    <templarg>typename Signature</templarg>
  </compound>
  <compound kind="struct">
    <name>kwk::view</name>
    <filename>structkwk_1_1view.html</filename>
    <templarg>typename Builder</templarg>
    <member kind="typedef">
      <type>typename parent::const_pointer</type>
      <name>const_pointer</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a7b61399e18244610e305e946fb39fcfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::const_reference</type>
      <name>const_reference</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a810df304d57adbd28e15472b37150b55</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::pointer</type>
      <name>pointer</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>ae70185b1e59a33924ea4e2032c2be65a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::reference</type>
      <name>reference</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a17801a3cd40872a38c204fd7dc3ed379</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::shape_type</type>
      <name>shape_type</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a7a1f24b9c101e77d63ac94dd00baf9b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>typename parent::value_type</type>
      <name>value_type</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a91ecc4e3ea69660ff102b7eaf22283d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a07c320dff7cf1796a618dd2c1efde81a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a1639c1a84c23d1b8003c1b2e01832c09</anchor>
      <arglist>(rbr::concepts::option auto const &amp;... opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a37a160f5aa12e18154b365331acb0cd6</anchor>
      <arglist>(rbr::concepts::settings auto const &amp;opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>ac9eeee6be3471d70804fd023eaea3a8b</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr view &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>ad8c92c029a6541df210fd12bead274aa</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr std::int32_t</type>
      <name>static_order</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a0dde26811869ff2c9e417d0f0c154736</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a07c320dff7cf1796a618dd2c1efde81a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a1639c1a84c23d1b8003c1b2e01832c09</anchor>
      <arglist>(rbr::concepts::option auto const &amp;... opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>a37a160f5aa12e18154b365331acb0cd6</anchor>
      <arglist>(rbr::concepts::settings auto const &amp;opts)</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>view</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>ac9eeee6be3471d70804fd023eaea3a8b</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
    <member kind="function">
      <type>constexpr view &amp;</type>
      <name>operator=</name>
      <anchorfile>structkwk_1_1view.html</anchorfile>
      <anchor>ad8c92c029a6541df210fd12bead274aa</anchor>
      <arglist>(concepts::container&lt; type&lt; value_type &gt;, shape_type &gt; auto const &amp;other)</arglist>
    </member>
  </compound>
  <compound kind="concept">
    <name>kwk::callable</name>
    <filename>conceptkwk_1_1callable.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::allocator</name>
    <filename>conceptkwk_1_1concepts_1_1allocator.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::axis</name>
    <filename>conceptkwk_1_1concepts_1_1axis.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::basic_container</name>
    <filename>conceptkwk_1_1concepts_1_1basic__container.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::category</name>
    <filename>conceptkwk_1_1concepts_1_1category.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::container</name>
    <filename>conceptkwk_1_1concepts_1_1container.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::contiguous_range</name>
    <filename>conceptkwk_1_1concepts_1_1contiguous__range.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::contiguous_static_range</name>
    <filename>conceptkwk_1_1concepts_1_1contiguous__static__range.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::dynamic_axis</name>
    <filename>conceptkwk_1_1concepts_1_1dynamic__axis.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::extent</name>
    <filename>conceptkwk_1_1concepts_1_1extent.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::has_static_size</name>
    <filename>conceptkwk_1_1concepts_1_1has__static__size.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::joker</name>
    <filename>conceptkwk_1_1concepts_1_1joker.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::named_extent</name>
    <filename>conceptkwk_1_1concepts_1_1named__extent.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::numeric_extent</name>
    <filename>conceptkwk_1_1concepts_1_1numeric__extent.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::pointer</name>
    <filename>conceptkwk_1_1concepts_1_1pointer.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::range</name>
    <filename>conceptkwk_1_1concepts_1_1range.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::slicer</name>
    <filename>conceptkwk_1_1concepts_1_1slicer.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::static_axis</name>
    <filename>conceptkwk_1_1concepts_1_1static__axis.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::table</name>
    <filename>conceptkwk_1_1concepts_1_1table.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::unit_slicer</name>
    <filename>conceptkwk_1_1concepts_1_1unit__slicer.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::concepts::view</name>
    <filename>conceptkwk_1_1concepts_1_1view.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::deferred_callable</name>
    <filename>conceptkwk_1_1deferred__callable.html</filename>
  </compound>
  <compound kind="concept">
    <name>kwk::tag_invocable</name>
    <filename>conceptkwk_1_1tag__invocable.html</filename>
  </compound>
  <compound kind="namespace">
    <name>kwk</name>
    <filename>namespacekwk.html</filename>
    <namespace>kwk::__</namespace>
    <namespace>kwk::tags</namespace>
    <class kind="struct">kwk::any_allocator</class>
    <class kind="struct">kwk::heap_allocator</class>
    <class kind="struct">kwk::is_dynamic_extent</class>
    <class kind="struct">kwk::is_joker</class>
    <class kind="struct">kwk::is_static_extent</class>
    <class kind="struct">kwk::joker</class>
    <class kind="struct">kwk::shape</class>
    <class kind="struct">kwk::stride</class>
    <class kind="struct">kwk::table</class>
    <class kind="struct">kwk::unsupported_call</class>
    <class kind="struct">kwk::view</class>
    <concept>kwk::callable</concept>
    <concept>kwk::deferred_callable</concept>
    <concept>kwk::tag_invocable</concept>
    <member kind="typedef">
      <type>table&lt; __::builder&lt; rbr::settings(table_, Settings{}...)&gt; &gt;</type>
      <name>make_table_t</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a2569ce77ae62fe68cd0cb4d7261eb2f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>view&lt; __::builder&lt; rbr::settings(view_, Settings{}...)&gt; &gt;</type>
      <name>make_view_t</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a431617994d8e4d0604155ef88f875cab</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::invoke_result_t&lt; decltype(kwk::tag_invoke), Tag, Args... &gt;</type>
      <name>tag_invoke_result</name>
      <anchorfile>group__invoke.html</anchorfile>
      <anchor>ga49771be90000328b8e6fb7e0330a41a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::decay_t&lt; decltype(Func)&gt;</type>
      <name>tag_of</name>
      <anchorfile>group__invoke.html</anchorfile>
      <anchor>ga05ec997ef2d1c398afbefdbab5223167</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>allocate</name>
      <anchorfile>namespacekwk_a897c0c275002e635427cf0c945c0915f.html</anchorfile>
      <anchor>a897c0c275002e635427cf0c945c0915f</anchor>
      <arglist>(any_allocator &amp;a, std::size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>allocate</name>
      <anchorfile>namespacekwk_a60dc6e232f493b54335349efefdf87c3.html</anchorfile>
      <anchor>a60dc6e232f493b54335349efefdf87c3</anchor>
      <arglist>(heap_allocator const &amp;, std::size_t n) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>as_stride</name>
      <anchorfile>namespacekwk_a0906c6f19e631291a05d91ed76d7da24.html</anchorfile>
      <anchor>a0906c6f19e631291a05d91ed76d7da24</anchor>
      <arglist>(shape&lt; D... &gt; s) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>compress</name>
      <anchorfile>namespacekwk_ac9d9ba85008a66e30a04039b551834b8.html</anchorfile>
      <anchor>ac9d9ba85008a66e30a04039b551834b8</anchor>
      <arglist>(shape&lt; D... &gt; const &amp;) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>compress</name>
      <anchorfile>namespacekwk_a84e058edb95a77ea7690c14f343b06d7.html</anchorfile>
      <anchor>a84e058edb95a77ea7690c14f343b06d7</anchor>
      <arglist>(shape&lt; D... &gt; const &amp;s) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>coordinates</name>
      <anchorfile>group__utility_ga824074c3406ec30c3c748dc0ce1260ac.html</anchorfile>
      <anchor>ga824074c3406ec30c3c748dc0ce1260ac</anchor>
      <arglist>(Idx idx, shape&lt; Desc... &gt; const shp) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deallocate</name>
      <anchorfile>namespacekwk_a6bfff93b539e2372e57e30dcbf35d4e2.html</anchorfile>
      <anchor>a6bfff93b539e2372e57e30dcbf35d4e2</anchor>
      <arglist>(any_allocator &amp;a, void *ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deallocate</name>
      <anchorfile>namespacekwk_a35509a947397e28b3bc6e673646b57a1.html</anchorfile>
      <anchor>a35509a947397e28b3bc6e673646b57a1</anchor>
      <arglist>(heap_allocator const &amp;, void *ptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>end_t</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a45cc145ad95771a2681d1f685bf185e5</anchor>
      <arglist>(O, R) -&gt; end_t&lt; O, R &gt;</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>for_each</name>
      <anchorfile>group__algorithms_ga16aa3f9107d2e0c6c06a728253bcfa9a.html</anchorfile>
      <anchor>ga16aa3f9107d2e0c6c06a728253bcfa9a</anchor>
      <arglist>(Func &amp;&amp;f, C0 &amp;&amp;c0, Cs &amp;&amp;... cs)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>for_each</name>
      <anchorfile>group__algorithms_ga19dd0b9a886b89ddb3f143f5c567729c.html</anchorfile>
      <anchor>ga19dd0b9a886b89ddb3f143f5c567729c</anchor>
      <arglist>(Func &amp;&amp;f, shape&lt; S... &gt; const &amp;shp)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>for_each_index</name>
      <anchorfile>group__algorithms_ga77ac360814df6bb39e9adfd4e3e9508b.html</anchorfile>
      <anchor>ga77ac360814df6bb39e9adfd4e3e9508b</anchor>
      <arglist>(Context &amp;ctx, Func &amp;&amp;f, Container &amp;&amp;c)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>linear_index</name>
      <anchorfile>group__utility_gae18793767c7f746a447cdb28c9237756.html</anchorfile>
      <anchor>gae18793767c7f746a447cdb28c9237756</anchor>
      <arglist>(shape&lt; S... &gt; const &amp;sh, Indexes const &amp;indexes) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>linear_index</name>
      <anchorfile>group__utility_gae66b60c6f375d79b5f9a1d7a70b27197.html</anchorfile>
      <anchor>gae66b60c6f375d79b5f9a1d7a70b27197</anchor>
      <arglist>(shape&lt; S... &gt; sh, Index... idx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>linear_index</name>
      <anchorfile>group__utility_gae2a07eb22b969f467b74ee3f0cf81de9.html</anchorfile>
      <anchor>gae2a07eb22b969f467b74ee3f0cf81de9</anchor>
      <arglist>(shape&lt; S... &gt; sh, Indexes idx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>of_size</name>
      <anchorfile>namespacekwk_af1fd6102366cab9dbb5f43470f227aac.html</anchorfile>
      <anchor>af1fd6102366cab9dbb5f43470f227aac</anchor>
      <arglist>(D... d)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>of_size</name>
      <anchorfile>namespacekwk_ad20200aac387b71b6a4f84e5c22113d8.html</anchorfile>
      <anchor>ad20200aac387b71b6a4f84e5c22113d8</anchor>
      <arglist>(D... d)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>of_size</name>
      <anchorfile>namespacekwk_af1f996fd89cf17548575f54fe60a263f.html</anchorfile>
      <anchor>af1f996fd89cf17548575f54fe60a263f</anchor>
      <arglist>(T const &amp;t)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>of_size</name>
      <anchorfile>namespacekwk_acd8b2649b4aff4149964f1fca254bd57.html</anchorfile>
      <anchor>acd8b2649b4aff4149964f1fca254bd57</anchor>
      <arglist>(T const &amp;t)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>operator&quot;&quot;_c</name>
      <anchorfile>group__utility_ga2ec3b967a56cb24a680d64cb87f2952c.html</anchorfile>
      <anchor>ga2ec3b967a56cb24a680d64cb87f2952c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>shape</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a4ba16b4bab496b5eb5af6e0e457fcfa5</anchor>
      <arglist>(T...) -&gt; shape&lt; to_descriptor(T{})... &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>stride</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>aa090076284bbdd664ad5562af3577722</anchor>
      <arglist>(T...) -&gt; stride&lt; to_descriptor(T{})... &gt;</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>with_strides</name>
      <anchorfile>namespacekwk_af811af24909a4a34f72e7bcff3b21f8f.html</anchorfile>
      <anchor>af811af24909a4a34f72e7bcff3b21f8f</anchor>
      <arglist>(D... d) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>table</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a236dd1c8c8f7cfeab0592cb9852af044</anchor>
      <arglist>(O const &amp;...) -&gt; table&lt; __::builder&lt; rbr::settings(table_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>table</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>aa37b8e64f34219a4950c708503fb799f</anchor>
      <arglist>(rbr::settings&lt; O... &gt; const &amp;) -&gt; table&lt; __::builder&lt; rbr::settings(table_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>table</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>ac1bab271cb3ece174f7ee5d28d44f5ea</anchor>
      <arglist>(C const &amp;) -&gt; table&lt; __::builder&lt; C::archetype(table_)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>view</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a9ec29968c1acb9f44d35cb468e6229ab</anchor>
      <arglist>(O const &amp;...) -&gt; view&lt; __::builder&lt; rbr::settings(view_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>view</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a7b781dbf5d8af2bfefde51aaa4038a03</anchor>
      <arglist>(rbr::settings&lt; O... &gt; const &amp;) -&gt; view&lt; __::builder&lt; rbr::settings(view_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>view</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a2b8003b8c7ad9a6be8942a3d9e5ed9d7</anchor>
      <arglist>(C const &amp;) -&gt; view&lt; __::builder&lt; C::archetype(view_)&gt; &gt;</arglist>
    </member>
    <member kind="variable">
      <type>constexpr joker</type>
      <name>_</name>
      <anchorfile>group__utility_ga5008705a9f99e5f4bf3eab64f12e8ac1.html</anchorfile>
      <anchor>ga5008705a9f99e5f4bf3eab64f12e8ac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::allocator_</type>
      <name>allocator</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gac4c926f0ac22e4c1aacc25d88325fc41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::axis_&lt; N &gt;</type>
      <name>along</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>abf80c3212de5864c8fbd74ebe3f42550</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr type&lt; T &gt;</type>
      <name>as</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gaaf19133293919817e89249657fca02e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::axis_&lt; ID &gt;</type>
      <name>axis</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a14a23cb4d4dbc3874abf2a17c2ff367c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::category_</type>
      <name>category</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga75c22e379a8fd072b0a8133d1da630cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>channel</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a84c0be3201cb42dd92a0986e2a2d9e20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>depth</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>adbe9690d935124a104a7760b4f93c81f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr end_t</type>
      <name>end</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a30790e2d96a846434c960bed0a4431ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>fixed</name>
      <anchorfile>group__utility.html</anchorfile>
      <anchor>gaac3d175a241803308de3f331028753e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::floor_</type>
      <name>floor</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a9733afb75003de91691984b8a4af4543</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>height</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>abf0a21038671dcb8af94763d6622059e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>int16</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>adbd328aa9e82a0ee607645b7fd64fb47</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>int32</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a04308cf65bf93d23987086e475f0b1d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>int64</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>ab69f3e46fbf9221c2e8edef4fe87508f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>int8</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>ae1a0fbb9847b2b2973593c7d4932c40b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::label_</type>
      <name>label</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gabb68e6f180ef839836e0dc210cd1772f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::linear_</type>
      <name>linear</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a4259f9c8fbae77136122e737945a85a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>real32</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a3b13b1bcba3359943146be636c0207da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>real64</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a1976486b798b854e789de80922a8b639</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::size_</type>
      <name>size</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga2846917ba45ea58670d9d45afe907925</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::source_</type>
      <name>source</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gaef8fdcbb44ad887e90784c21469e06eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::strides_</type>
      <name>strides</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga9f6509c2ec5668331e3d18e48ec35478</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::table_</type>
      <name>table_</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a73b06e6568836ced3f015cda983dcff4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>uint16</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>ab8a19b45d256ce1a4a572150004d813d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>uint32</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a1ec6a92e4a523254a8dce7478a563cf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>uint64</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a7d3f09d48d522d3c4bac37b976486be7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>uint8</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a0f71b86ce8ff3d8509d9f6d3ae2c3aca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>unreachable</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga803bd4ac5bdee7e3427f12d84805bd62</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::view_</type>
      <name>view_</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>af8672f59bb148e9e3840296bab16c231</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>width</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a8e5bc23eeee850fe9161ae7972dfc69e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>table</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a236dd1c8c8f7cfeab0592cb9852af044</anchor>
      <arglist>(O const &amp;...) -&gt; table&lt; __::builder&lt; rbr::settings(table_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>table</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>aa37b8e64f34219a4950c708503fb799f</anchor>
      <arglist>(rbr::settings&lt; O... &gt; const &amp;) -&gt; table&lt; __::builder&lt; rbr::settings(table_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>table</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>ac1bab271cb3ece174f7ee5d28d44f5ea</anchor>
      <arglist>(C const &amp;) -&gt; table&lt; __::builder&lt; C::archetype(table_)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>view</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a9ec29968c1acb9f44d35cb468e6229ab</anchor>
      <arglist>(O const &amp;...) -&gt; view&lt; __::builder&lt; rbr::settings(view_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>view</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a7b781dbf5d8af2bfefde51aaa4038a03</anchor>
      <arglist>(rbr::settings&lt; O... &gt; const &amp;) -&gt; view&lt; __::builder&lt; rbr::settings(view_, O{}...)&gt; &gt;</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>view</name>
      <anchorfile>namespacekwk.html</anchorfile>
      <anchor>a2b8003b8c7ad9a6be8942a3d9e5ed9d7</anchor>
      <arglist>(C const &amp;) -&gt; view&lt; __::builder&lt; C::archetype(view_)&gt; &gt;</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>kwk::__</name>
    <filename>namespacekwk_1_1____.html</filename>
  </compound>
  <compound kind="namespace">
    <name>kwk::tags</name>
    <filename>namespacekwk_1_1tags.html</filename>
  </compound>
  <compound kind="group">
    <name>extension</name>
    <title>Advanced User Extension Points</title>
    <filename>group__extension.html</filename>
  </compound>
  <compound kind="group">
    <name>algos</name>
    <title>Algorithms</title>
    <filename>group__algos.html</filename>
  </compound>
  <compound kind="group">
    <name>algorithms</name>
    <title>Algorithms and View generators</title>
    <filename>group__algorithms.html</filename>
    <subgroup>algos</subgroup>
    <subgroup>algo_views</subgroup>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::for_each</name>
      <anchorfile>group__algorithms_ga16aa3f9107d2e0c6c06a728253bcfa9a.html</anchorfile>
      <anchor>ga16aa3f9107d2e0c6c06a728253bcfa9a</anchor>
      <arglist>(Func &amp;&amp;f, C0 &amp;&amp;c0, Cs &amp;&amp;... cs)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::for_each</name>
      <anchorfile>group__algorithms_ga19dd0b9a886b89ddb3f143f5c567729c.html</anchorfile>
      <anchor>ga19dd0b9a886b89ddb3f143f5c567729c</anchor>
      <arglist>(Func &amp;&amp;f, shape&lt; S... &gt; const &amp;shp)</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::for_each_index</name>
      <anchorfile>group__algorithms_ga77ac360814df6bb39e9adfd4e3e9508b.html</anchorfile>
      <anchor>ga77ac360814df6bb39e9adfd4e3e9508b</anchor>
      <arglist>(Context &amp;ctx, Func &amp;&amp;f, Container &amp;&amp;c)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>settings</name>
    <title>Container Settings</title>
    <filename>group__settings.html</filename>
    <member kind="variable">
      <type>constexpr __::allocator_</type>
      <name>kwk::allocator</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gac4c926f0ac22e4c1aacc25d88325fc41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr type&lt; T &gt;</type>
      <name>kwk::as</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gaaf19133293919817e89249657fca02e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::category_</type>
      <name>kwk::category</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga75c22e379a8fd072b0a8133d1da630cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::label_</type>
      <name>kwk::label</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gabb68e6f180ef839836e0dc210cd1772f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::size_</type>
      <name>kwk::size</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga2846917ba45ea58670d9d45afe907925</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::source_</type>
      <name>kwk::source</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>gaef8fdcbb44ad887e90784c21469e06eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr __::strides_</type>
      <name>kwk::strides</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga9f6509c2ec5668331e3d18e48ec35478</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>kwk::unreachable</name>
      <anchorfile>group__settings.html</anchorfile>
      <anchor>ga803bd4ac5bdee7e3427f12d84805bd62</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>containers</name>
    <title>Containers</title>
    <filename>group__containers.html</filename>
    <subgroup>settings</subgroup>
    <class kind="struct">kwk::shape</class>
    <class kind="struct">kwk::stride</class>
    <class kind="struct">kwk::table</class>
    <class kind="struct">kwk::view</class>
  </compound>
  <compound kind="group">
    <name>contexts</name>
    <title>Contexts</title>
    <filename>group__contexts.html</filename>
  </compound>
  <compound kind="group">
    <name>invoke</name>
    <title>Generalized Tag Invoke Protocol</title>
    <filename>group__invoke.html</filename>
    <class kind="struct">kwk::unsupported_call</class>
    <concept>kwk::callable</concept>
    <concept>kwk::deferred_callable</concept>
    <concept>kwk::tag_invocable</concept>
    <member kind="typedef">
      <type>std::invoke_result_t&lt; decltype(kwk::tag_invoke), Tag, Args... &gt;</type>
      <name>kwk::tag_invoke_result</name>
      <anchorfile>group__invoke.html</anchorfile>
      <anchor>ga49771be90000328b8e6fb7e0330a41a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::decay_t&lt; decltype(Func)&gt;</type>
      <name>kwk::tag_of</name>
      <anchorfile>group__invoke.html</anchorfile>
      <anchor>ga05ec997ef2d1c398afbefdbab5223167</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>utility</name>
    <title>Miscellaneous Helpers</title>
    <filename>group__utility.html</filename>
    <subgroup>extension</subgroup>
    <subgroup>invoke</subgroup>
    <subgroup>traits</subgroup>
    <class kind="struct">kwk::joker</class>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::coordinates</name>
      <anchorfile>group__utility_ga824074c3406ec30c3c748dc0ce1260ac.html</anchorfile>
      <anchor>ga824074c3406ec30c3c748dc0ce1260ac</anchor>
      <arglist>(Idx idx, shape&lt; Desc... &gt; const shp) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::linear_index</name>
      <anchorfile>group__utility_gae18793767c7f746a447cdb28c9237756.html</anchorfile>
      <anchor>gae18793767c7f746a447cdb28c9237756</anchor>
      <arglist>(shape&lt; S... &gt; const &amp;sh, Indexes const &amp;indexes) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::linear_index</name>
      <anchorfile>group__utility_gae66b60c6f375d79b5f9a1d7a70b27197.html</anchorfile>
      <anchor>gae66b60c6f375d79b5f9a1d7a70b27197</anchor>
      <arglist>(shape&lt; S... &gt; sh, Index... idx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::linear_index</name>
      <anchorfile>group__utility_gae2a07eb22b969f467b74ee3f0cf81de9.html</anchorfile>
      <anchor>gae2a07eb22b969f467b74ee3f0cf81de9</anchor>
      <arglist>(shape&lt; S... &gt; sh, Indexes idx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr auto</type>
      <name>kwk::literals::operator&quot;&quot;_c</name>
      <anchorfile>group__utility_ga2ec3b967a56cb24a680d64cb87f2952c.html</anchorfile>
      <anchor>ga2ec3b967a56cb24a680d64cb87f2952c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable">
      <type>constexpr joker</type>
      <name>kwk::_</name>
      <anchorfile>group__utility_ga5008705a9f99e5f4bf3eab64f12e8ac1.html</anchorfile>
      <anchor>ga5008705a9f99e5f4bf3eab64f12e8ac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr auto</type>
      <name>kwk::fixed</name>
      <anchorfile>group__utility.html</anchorfile>
      <anchor>gaac3d175a241803308de3f331028753e6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>traits</name>
    <title>Type Traits</title>
    <filename>group__traits.html</filename>
    <class kind="struct">kwk::is_dynamic_extent</class>
    <class kind="struct">kwk::is_joker</class>
    <class kind="struct">kwk::is_static_extent</class>
  </compound>
  <compound kind="group">
    <name>algo_views</name>
    <title>View Generators</title>
    <filename>group__algo__views.html</filename>
  </compound>
  <compound kind="page">
    <name>md__2____w_2kiwaku_2kiwaku_2doc_2index</name>
    <title>Kiwaku - Containers Well Made</title>
    <filename>md__2____w_2kiwaku_2kiwaku_2doc_2index.html</filename>
  </compound>
  <compound kind="page">
    <name>about</name>
    <title>Installation and Quick Start</title>
    <filename>about.html</filename>
    <docanchor file="about.html" title="Installation &amp; Quick Start">install</docanchor>
  </compound>
  <compound kind="page">
    <name>tutorials</name>
    <title>Tutorials</title>
    <filename>tutorials.html</filename>
    <subpage>tutorial-01.html</subpage>
  </compound>
  <compound kind="page">
    <name>rationale</name>
    <title>Why and How</title>
    <filename>rationale.html</filename>
    <subpage>glossary.html</subpage>
  </compound>
  <compound kind="page">
    <name>changelog</name>
    <title>Change Log</title>
    <filename>changelog.html</filename>
    <docanchor file="changelog.html" title="Version 0.1 (beta)">v01</docanchor>
  </compound>
  <compound kind="page">
    <name>licence</name>
    <title>Licence</title>
    <filename>licence.html</filename>
  </compound>
  <compound kind="page">
    <name>glossary</name>
    <title>Glossary</title>
    <filename>glossary.html</filename>
    <docanchor file="glossary.html" title="Shape-related Terms">glossary-shape</docanchor>
    <docanchor file="glossary.html" title="Axis">glossary-axis</docanchor>
    <docanchor file="glossary.html" title="Extent">glossary-extent</docanchor>
    <docanchor file="glossary.html" title="Container-related Terms">glossary-container</docanchor>
    <docanchor file="glossary.html" title="Order">glossary-order</docanchor>
  </compound>
  <compound kind="page">
    <name>tutorial-01</name>
    <title>Multi-dimensional views</title>
    <filename>tutorial-01.html</filename>
  </compound>
</tagfile>
