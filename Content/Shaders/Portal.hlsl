
float2 Resolution = {1.0f, 1.0f};

float2 p = {2.0f * FragCoord.xy / Resolution.xy - 1.0f};
float2 ratio = {Resolution.x / Resolution.y, 1.0f};
p = p * ratio;

float2 uv = {atan(p.y/ p.x) * 1.0f/3.14f, 1.0f / sqrt(dot(p, p))};
float2 scale = {2.0f, 1.0f};
uv = uv * scale;

uv.x += sin(ImageWarp * uv.y + Time * WarpSpeed);

float3 colour = Texture2DSample(Tex, TexSampler, uv).xyz/ (uv.y * Darkness + 1.0);

return colour;