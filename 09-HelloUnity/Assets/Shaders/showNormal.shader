Shader "Sketchbook/normal" {
	Properties {
		// _MainTex ("Base (RGB)", 2D) = "white" {}
	    // _Color ("Main Color", Color) = (1,1,1,1)
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Lambert vertex:vert

		// sampler2D _MainTex;
		// half4 _Color;

		struct Input {
			float2 uv_MainTex;
			half3 debugColor;
		};

		void vert(inout appdata_full v, out Input o)
		{
			//v.normal = normalize(1, 0.5, 0);
			o.debugColor = ( v.normal.xyz * 0.5 ) + 0.5;
			//o.debugColor = half3(0, 1, 0);
		}

		void surf (Input IN, inout SurfaceOutput o) {
			// half4 c = tex2D (_MainTex, IN.uv_MainTex);
			//c *= _Color;

			half4 c = (1,1,1,1);
			c.rgb = IN.debugColor.rgb;

			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
		ENDCG
	} 
	FallBack "Diffuse"
}
