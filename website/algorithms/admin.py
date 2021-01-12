from django.contrib import admin
from .models import Algorithm, Outcome, Benchmark

# Register your models here.

admin.site.register(Algorithm)
admin.site.register(Outcome)
admin.site.register(Benchmark)
